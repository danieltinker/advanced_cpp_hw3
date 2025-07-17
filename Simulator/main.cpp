// Simulator/main.cpp

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <dlfcn.h>
#include <stdexcept>

#include "ArgParser.hpp"
#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"
#include "ThreadPool.hpp"
#include "SatelliteView.h"
#include "GameResult.h"

namespace fs = std::filesystem;

//------------------------------------------------------------------------------
// MapLoader: parse your assignment‐style map file
//------------------------------------------------------------------------------
struct MapData {
    std::unique_ptr<SatelliteView> view;
    size_t rows, cols;
    size_t maxSteps, numShells;
};

static MapData loadMapWithParams(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open map file: " + path);
    }

    size_t rows = 0, cols = 0, maxSteps = 0, numShells = 0;
    std::vector<std::string> gridLines;
    std::string line;

    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.rfind("Rows",      0) == 0) {
            rows = std::stoul(line.substr(line.find('=') + 1));
        } else if (line.rfind("Cols",      0) == 0) {
            cols = std::stoul(line.substr(line.find('=') + 1));
        } else if (line.rfind("MaxSteps",  0) == 0) {
            maxSteps = std::stoul(line.substr(line.find('=') + 1));
        } else if (line.rfind("NumShells", 0) == 0) {
            numShells = std::stoul(line.substr(line.find('=') + 1));
        } else {
            if (line.empty()) continue;
            bool isGrid = true;
            for (char c : line) {
                if (c!='.' && c!='#' && c!='@' && c!='1' && c!='2') {
                    isGrid = false;
                    break;
                }
            }
            if (isGrid) gridLines.push_back(line);
        }
    }

    if (rows==0 || cols==0)
        throw std::runtime_error("Missing Rows or Cols in map header");
    if (gridLines.size() != rows) {
        std::ostringstream os;
        os << "Expected " << rows << " grid lines but found " << gridLines.size();
        throw std::runtime_error(os.str());
    }
    for (size_t i = 0; i < rows; ++i) {
        if (gridLines[i].size() != cols) {
            std::ostringstream os;
            os << "Map row " << i << " length " << gridLines[i].size()
               << " != Cols=" << cols;
            throw std::runtime_error(os.str());
        }
    }

    // Build SatelliteView:
    class MapView : public SatelliteView {
    public:
        MapView(std::vector<std::string>&& rows)
          : rows_(std::move(rows)),
            width_(rows_.empty() ? 0 : rows_[0].size()),
            height_(rows_.size()) {}
        char getObjectAt(size_t x, size_t y) const override {
            return (y<height_ && x<width_) ? rows_[y][x] : ' ';
        }
        size_t width()  const { return width_;  }
        size_t height() const { return height_; }
    private:
        std::vector<std::string> rows_;
        size_t width_, height_;
    };

    MapData md;
    md.rows      = rows;
    md.cols      = cols;
    md.maxSteps  = maxSteps;
    md.numShells = numShells;
    md.view      = std::make_unique<MapView>(std::move(gridLines));
    return md;
}

// strip “.so” and directory from a path
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
    // 1) Load map + params
    MapData md;
    try {
        md = loadMapWithParams(cfg.game_map);
    } catch (const std::exception& ex) {
        std::cerr << "Error loading map: " << ex.what() << "\n";
        return 1;
    }
    SatelliteView& realMap = *md.view;

    // 2) Load Algorithms
    auto& algoReg = AlgorithmRegistrar::get();
    std::vector<void*> algoHandles;
    for (auto const& algPath : {cfg.algorithm1, cfg.algorithm2}) {
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

    // 3) Load GameManagers
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

    // 4) Dispatch tasks
    ThreadPool pool(cfg.numThreads);
    std::mutex mtx;
    struct Entry {
        std::string gm, a1, a2;
        GameResult res;
        Entry(std::string g, std::string x, std::string y, GameResult r)
          : gm(std::move(g)), a1(std::move(x)), a2(std::move(y)), res(std::move(r)) {}
    };
    std::vector<Entry> results;

    for (size_t gi = 0; gi < gmPaths.size(); ++gi) {
        auto& gmEntry = *(gmReg.begin() + gi);
        auto& A = *(algoReg.begin() + 0);
        auto& B = *(algoReg.begin() + 1);

        pool.enqueue([&, gi] {
            auto gm = gmEntry.factory(cfg.verbose);
            auto p1 = A.createPlayer(0, 0, 0, md.maxSteps, md.numShells);
            auto a1 = A.createTankAlgorithm(0, 0);
            auto p2 = B.createPlayer(1, 0, 0, md.maxSteps, md.numShells);
            auto a2 = B.createTankAlgorithm(1, 0);

            GameResult gr = gm->run(
                md.cols, md.rows,
                realMap,
                cfg.game_map,
                md.maxSteps, md.numShells,
                *p1, stripSo(cfg.algorithm1),
                *p2, stripSo(cfg.algorithm2),
                [&](int pi,int ti){ return A.createTankAlgorithm(pi,ti); },
                [&](int pi,int ti){ return B.createTankAlgorithm(pi,ti); }
            );

            std::lock_guard<std::mutex> lock(mtx);
            results.emplace_back(
                stripSo(gmPaths[gi]),
                stripSo(cfg.algorithm1),
                stripSo(cfg.algorithm2),
                std::move(gr)
            );
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
                  << "  rounds=" << e.res.rounds << "\n";
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

    // 3) Load Algos
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

    // 4) Preload maps into shared_ptrs so lambdas can capture safely
    std::vector<std::shared_ptr<SatelliteView>> mapViews;
    std::vector<size_t>                         mapRows, mapCols, mapMaxSteps, mapNumShells;
    for (auto const& mapFile : maps) {
        try {
            MapData md = loadMapWithParams(mapFile);
            mapViews.emplace_back(std::move(md.view));
            mapCols .push_back(md.cols);
            mapRows .push_back(md.rows);
            mapMaxSteps.push_back(md.maxSteps);
            mapNumShells.push_back(md.numShells);
        } catch (const std::exception& ex) {
            std::cerr << "Warning: skipping map '" << mapFile << "': " << ex.what() << "\n";
        }
    }
    if (mapViews.empty()) {
        std::cerr << "Error: no valid maps to run\n";
        dlclose(gmH);
        return 1;
    }

    // 5) Dispatch tasks
    ThreadPool pool(cfg.numThreads);
    std::mutex mtx;
    struct Entry {
        std::string mapFile, a1, a2;
        GameResult res;
        Entry(std::string m, std::string x, std::string y, GameResult r)
          : mapFile(std::move(m)), a1(std::move(x)), a2(std::move(y)), res(std::move(r)) {}
    };
    std::vector<Entry> results;
    auto& gmEntry = *gmReg.begin();

    for (size_t mi = 0; mi < mapViews.size(); ++mi) {
        auto mapViewPtr = mapViews[mi];
        size_t cols     = mapCols[mi],
               rows     = mapRows[mi],
               mSteps   = mapMaxSteps[mi],
               nShells  = mapNumShells[mi];
        const std::string mapFile = maps[mi];
        SatelliteView& realMap = *mapViewPtr;

        for (size_t i = 0; i + 1 < algoPaths.size(); ++i) {
            for (size_t j = i + 1; j < algoPaths.size(); ++j) {
                pool.enqueue([=,&realMap,&mtx,&results,&algoReg,&gmEntry]() {
                    auto gm = gmEntry.factory(cfg.verbose);
                    auto& A = *(algoReg.begin() + i);
                    auto& B = *(algoReg.begin() + j);
                    auto p1 = A.createPlayer(0,0,0,mSteps,nShells);
                    auto a1 = A.createTankAlgorithm(0,0);
                    auto p2 = B.createPlayer(1,0,0,mSteps,nShells);
                    auto a2 = B.createTankAlgorithm(1,0);

                    GameResult gr = gm->run(
                        cols, rows,
                        realMap,
                        mapFile,
                        mSteps, nShells,
                        *p1, stripSo(algoPaths[i]),
                        *p2, stripSo(algoPaths[j]),
                        [&](int pi,int ti){ return A.createTankAlgorithm(pi,ti); },
                        [&](int pi,int ti){ return B.createTankAlgorithm(pi,ti); }
                    );

                    std::lock_guard<std::mutex> lock(mtx);
                    results.emplace_back(
                        mapFile,
                        stripSo(algoPaths[i]),
                        stripSo(algoPaths[j]),
                        std::move(gr)
                    );
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
                  << " => winner=" << e.res.winner
                  << "  reason=" << static_cast<int>(e.res.reason)
                  << "  rounds=" << e.res.rounds << "\n";
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
