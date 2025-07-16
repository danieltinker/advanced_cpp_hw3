// Simulator/main.cpp

#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include <vector>
#include <string>
#include <mutex>

#include "ArgParser.hpp"
#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"
#include "ThreadPool.hpp"
#include <SatelliteView.h>
#include <GameResult.h>

namespace fs = std::filesystem;

// Helper: strip “.so” and directory from a path
static std::string stripSo(const std::string& path) {
    auto fname = fs::path(path).filename().string();
    if (auto pos = fname.rfind(".so"); pos != std::string::npos)
        return fname.substr(0, pos);
    return fname;
}

// -----------------------------
// Comparative mode
// -----------------------------
static int runComparative(const Config& cfg) {
    // 1) Load Algorithms
    auto& algoReg = AlgorithmRegistrar::get();
    std::vector<void*> algoHandles;
    for (auto const& algPath : { cfg.algorithm1, cfg.algorithm2 }) {
        std::string name = stripSo(algPath);
        algoReg.createAlgorithmFactoryEntry(name);
        void* h = dlopen(algPath.c_str(), RTLD_NOW);
        if (!h) {
            std::cerr << "Error: dlopen Algo '" << name << "' failed: " << dlerror() << "\n";
            return 1;
        }
        try { algoReg.validateLastRegistration(); }
        catch (...) {
            std::cerr << "Error: Algo registration failed for '" << name << "'\n";
            algoReg.removeLast();
            dlclose(h);
            return 1;
        }
        algoHandles.push_back(h);
    }

    // 2) Load GameManagers from folder
    auto& gmReg = GameManagerRegistrar::get();
    std::vector<std::string> gmPaths;
    for (auto& e : fs::directory_iterator(cfg.game_managers_folder))
        if (e.path().extension() == ".so")
            gmPaths.push_back(e.path().string());
    if (gmPaths.empty()) {
        std::cerr << "Error: no .so in game_managers_folder\n";
        return 1;
    }

    std::vector<void*> gmHandles;
    for (auto const& gmPath : gmPaths) {
        std::string name = stripSo(gmPath);
        gmReg.createGameManagerEntry(name);
        void* h = dlopen(gmPath.c_str(), RTLD_NOW);
        if (!h) {
            std::cerr << "Error: dlopen GM '" << name << "' failed: " << dlerror() << "\n";
            return 1;
        }
        try { gmReg.validateLastRegistration(); }
        catch (...) {
            std::cerr << "Error: GM registration failed for '" << name << "'\n";
            gmReg.removeLast();
            dlclose(h);
            return 1;
        }
        gmHandles.push_back(h);
    }

    // 3) Dummy view so we don't dereference nullptr
    struct DummyView : public SatelliteView {
        char getObjectAt(size_t, size_t) const override { return ' '; }
    } dummyView;

    // 4) Dispatch tasks
    ThreadPool pool(cfg.numThreads);
    std::mutex               mtx;
    struct Entry { std::string gm, a1, a2; GameResult res; };
    std::vector<Entry>       results;

    // We know exactly two algos at indices 0,1
    for (size_t gi = 0; gi < gmPaths.size(); ++gi) {
        auto& gmEntry = *(gmReg.begin() + gi);
        auto& A       = *(algoReg.begin() + 0);
        auto& B       = *(algoReg.begin() + 1);

        pool.enqueue([&, gi] {
            auto gm = gmEntry.factory(cfg.verbose);
            auto p1 = A.createPlayer(0,0,0,0,0);
            auto a1 = A.createTankAlgorithm(0,0);
            auto p2 = B.createPlayer(1,0,0,0,0);
            auto a2 = B.createTankAlgorithm(1,0);

            GameResult gr = gm->run(
                /*map_w=*/0, /*map_h=*/0,
                dummyView,
                cfg.game_map,
                /*max_steps=*/0, /*num_shells=*/0,
                *p1, stripSo(cfg.algorithm1),
                *p2, stripSo(cfg.algorithm2),
                [&](int pi,int ti){ return A.createTankAlgorithm(pi,ti); },
                [&](int pi,int ti){ return B.createTankAlgorithm(pi,ti); }
            );

            std::lock_guard lk(mtx);
            results.push_back({ stripSo(gmPaths[gi]),
                                stripSo(cfg.algorithm1),
                                stripSo(cfg.algorithm2),
                                gr });
        });
    }
    pool.shutdown();

    // 5) Report & cleanup
    std::cout << "[Simulator] Comparative Results:\n";
    for (auto& e : results) {
        std::cout << "  GM=" << e.gm
                  << "  A1=" << e.a1
                  << "  A2=" << e.a2
                  << " => winner="  << e.res.winner
                  << "  reason="  << static_cast<int>(e.res.reason)
                  << "\n";
    }
    // for (auto h : gmHandles)   dlclose(h);
    // for (auto h : algoHandles) dlclose(h);
    return 0;
}

// -----------------------------
// Competition mode
// -----------------------------
static int runCompetition(const Config& cfg) {
    // 1) Gather maps
    std::vector<std::string> maps;
    for (auto& e : fs::directory_iterator(cfg.game_maps_folder))
        if (e.is_regular_file())
            maps.push_back(e.path().string());
    if (maps.empty()) {
        std::cerr << "Error: no files in game_maps_folder\n";
        return 1;
    }

    // 2) Load GM
    auto& gmReg = GameManagerRegistrar::get();
    std::string gmName = stripSo(cfg.game_manager);
    gmReg.createGameManagerEntry(gmName);
    void* gmH = dlopen(cfg.game_manager.c_str(), RTLD_NOW);
    if (!gmH) {
        std::cerr << "Error: dlopen GM failed: " << dlerror() << "\n";
        return 1;
    }
    try { gmReg.validateLastRegistration(); }
    catch (...) {
        std::cerr << "Error: GM registration failed for '" << gmName << "'\n";
        gmReg.removeLast();
        dlclose(gmH);
        return 1;
    }

    // 3) Load Algos from folder
    auto& algoReg = AlgorithmRegistrar::get();
    std::vector<void*>    algoHandles;
    std::vector<std::string> algoPaths;
    for (auto& e : fs::directory_iterator(cfg.algorithms_folder)) {
        if (e.path().extension() == ".so") {
            std::string path = e.path().string();
            algoReg.createAlgorithmFactoryEntry(stripSo(path));
            void* h = dlopen(path.c_str(), RTLD_NOW);
            if (!h) {
                std::cerr << "Warning: dlopen Algo '" << path << "' failed\n";
                algoReg.removeLast();
                continue;
            }
            try { algoReg.validateLastRegistration(); }
            catch (...) {
                std::cerr << "Warning: Algo registration failed for '" << path << "'\n";
                algoReg.removeLast();
                dlclose(h);
                continue;
            }
            algoHandles.push_back(h);
            algoPaths.push_back(path);
        }
    }
    if (algoPaths.size() < 2) {
        std::cerr << "Error: need at least 2 algorithms in folder\n";
        dlclose(gmH);
        return 1;
    }

    // 4) Dummy view again
    struct DummyView : public SatelliteView {
        char getObjectAt(size_t, size_t) const override { return ' '; }
    } dummyView;

    // 5) Dispatch tasks
    ThreadPool pool(cfg.numThreads);
    std::mutex               mtx;
    struct Entry { std::string mapFile, a1, a2; GameResult res; };
    std::vector<Entry>       results;
    auto& gmEntry = *gmReg.begin();

    for (auto& mapFile : maps) {
        for (size_t i = 0; i + 1 < algoPaths.size(); ++i) {
            for (size_t j = i + 1; j < algoPaths.size(); ++j) {
                pool.enqueue([&, mapFile, i, j] {
                    auto gm = gmEntry.factory(cfg.verbose);
                    auto& A = *(algoReg.begin() + i);
                    auto& B = *(algoReg.begin() + j);
                    auto p1 = A.createPlayer(0,0,0,0,0);
                    auto a1 = A.createTankAlgorithm(0,0);
                    auto p2 = B.createPlayer(1,0,0,0,0);
                    auto a2 = B.createTankAlgorithm(1,0);

                    GameResult gr = gm->run(
                        /*map_w*/0, /*map_h*/0,
                        dummyView,
                        mapFile,
                        /*max_steps*/0, /*num_shells*/0,
                        *p1, stripSo(algoPaths[i]),
                        *p2, stripSo(algoPaths[j]),
                        [&](int pi,int ti){return A.createTankAlgorithm(pi,ti);},
                        [&](int pi,int ti){return B.createTankAlgorithm(pi,ti);}
                    );

                    std::lock_guard lk(mtx);
                    results.push_back({ mapFile,
                                        stripSo(algoPaths[i]),
                                        stripSo(algoPaths[j]),
                                        gr });
                });
            }
        }
    }
    pool.shutdown();

    // 6) Report & cleanup
    std::cout << "[Simulator] Competition Results:\n";
    for (auto& e : results) {
        std::cout << "  map=" << e.mapFile
                  << "  A1=" << e.a1
                  << "  A2=" << e.a2
                  << " => winner="  << e.res.winner
                  << "  reason="  << static_cast<int>(e.res.reason)
                  << "\n";
    }

    // dlclose(gmH);
    // for (auto h : algoHandles) dlclose(h);
    return 0;
}

// -----------------------------
// main()
// -----------------------------
int main(int argc, char* argv[]) {
    Config cfg;
    if (!parseArguments(argc, argv, cfg)) {
        return 1;
    }
    return cfg.modeComparative
        ? runComparative(cfg)
        : runCompetition(cfg);
}
