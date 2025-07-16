#include <getopt.h>
#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>

#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"

void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog << " <-comparative|--competition> \\\n"
              << "       --map <map_file> \\\n"
              << "       --manager <path_to_game_manager_so> \\\n"
              << "       --algos <path_to_algo1_so>[,<path_to_algo2_so>...] \\\n"
              << "       [--threads N] [--verbose]\n";
}

// Helper to turn "/path/to/Foo.so" into "Foo"
static std::string stripSo(const std::string& path) {
    auto slash = path.find_last_of("/\\");
    std::string file = (slash==std::string::npos ? path : path.substr(slash+1));
    auto dot   = file.rfind(".so");
    return (dot==std::string::npos ? file : file.substr(0, dot));
}

int main(int argc, char* argv[]) {
    std::string mapFile, managerSo;
    std::vector<std::string> algoSos;
    bool modeComparative = false, modeCompetition = false;
    int  numThreads     = 1;
    bool verbose        = false;

    static struct option longOpts[] = {
        { "comparative", no_argument,       nullptr, 'c' },
        { "competition",  no_argument,       nullptr, 'p' },
        { "map",          required_argument, nullptr, 'm' },
        { "manager",      required_argument, nullptr, 'g' },
        { "algos",        required_argument, nullptr, 'a' },
        { "threads",      required_argument, nullptr, 't' },
        { "verbose",      no_argument,       nullptr, 'v' },
        { nullptr,        0,                 nullptr,   0  }
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "cpm:g:a:t:v", longOpts, nullptr)) != -1) {
        switch (opt) {
            case 'c': modeComparative = true;            break;
            case 'p': modeCompetition  = true;            break;
            case 'm': mapFile          = optarg;         break;
            case 'g': managerSo        = optarg;         break;
            case 'a': {
                std::string list(optarg);
                size_t pos = 0;
                while ((pos = list.find(',')) != std::string::npos) {
                    algoSos.push_back(list.substr(0, pos));
                    list.erase(0, pos + 1);
                }
                if (!list.empty()) algoSos.push_back(list);
                break;
            }
            case 't': numThreads = std::stoi(optarg);    break;
            case 'v': verbose    = true;                 break;
            default:
                printUsage(argv[0]);
                return 1;
        }
    }

    // Validate mode
    if (modeComparative == modeCompetition) {
        std::cerr << "Error: must specify exactly one of --comparative or --competition\n";
        printUsage(argv[0]);
        return 1;
    }
    // Validate required args
    if (mapFile.empty() || managerSo.empty() || algoSos.empty()) {
        std::cerr << "Error: --map, --manager, and --algos are required\n";
        printUsage(argv[0]);
        return 1;
    }

    if (verbose) {
        std::cerr << "DEBUG: Mode               = "
                  << (modeComparative ? "Comparative" : "Competition") << "\n"
                  << "DEBUG: Map file           = " << mapFile << "\n"
                  << "DEBUG: Manager SO path    = " << managerSo << "\n"
                  << "DEBUG: Algorithm SO paths = ";
        for (auto& s : algoSos) std::cerr << s << " ";
        std::cerr << "\nDEBUG: Threads            = " << numThreads << "\n";
    }

    // 1) Dynamic‐load the GameManager
    {
        auto& gmReg = GameManagerRegistrar::getGameManagerRegistrar();
        std::string name = stripSo(managerSo);
        gmReg.createGameManagerEntry(name);

        if (verbose) std::cerr << "[Simulator] Loading GM plugin: " << managerSo << "\n";
        void* gmHandle = dlopen(managerSo.c_str(), RTLD_NOW);
        if (!gmHandle) {
            std::cerr << "Error: dlopen GM failed: " << dlerror() << "\n";
            return 1;
        }

        try {
            gmReg.validateLastRegistration();
        } catch (const std::exception& e) {
            std::cerr << "Error: GM registration for '" << name << "' failed\n";
            gmReg.removeLast();
            dlclose(gmHandle);
            return 1;
        }
    }

    // 2) Dynamic‐load each Algorithm
    {
        auto& algoReg = AlgorithmRegistrar::getAlgorithmRegistrar();
        for (auto& path : algoSos) {
            std::string name = stripSo(path);
            algoReg.createAlgorithmFactoryEntry(name);

            if (verbose) std::cerr << "[Simulator] Loading Algo plugin: " << path << "\n";
            void* handle = dlopen(path.c_str(), RTLD_NOW);
            if (!handle) {
                std::cerr << "Warning: dlopen Algo '" << name << "' failed: " << dlerror() << "\n";
                algoReg.removeLast();
                continue;
            }

            try {
                algoReg.validateLastRegistration();
            } catch (...) {
                std::cerr << "Warning: Algo registration for '" << name << "' failed\n";
                algoReg.removeLast();
                dlclose(handle);
            }
        }
    }

    std::cout << "[Simulator] CLI parsed and plugins loaded successfully.\n";
    return 0;
}
