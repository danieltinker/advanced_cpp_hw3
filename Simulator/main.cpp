#include <getopt.h>
#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>

#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"  // once implemented

void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog << " <-comparative|--competition> \\\n"
              << "       --map <map_file> \\\n"
              << "       --manager <path_to_game_manager_so> \\\n"
              << "       --algos <path_to_algo1_so>[,<path_to_algo2_so>...] \\\n"
              << "       [--threads N] [--verbose]\n";
}

int main(int argc, char* argv[]) {
    std::string mapFile;
    std::string managerSo;
    std::vector<std::string> algoSos;
    bool modeComparative = false;
    bool modeCompetition  = false;
    int  numThreads       = 1;
    bool verbose          = false;

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
        std::cout << "parsed opt: " << static_cast<char>(opt) << std::endl;
        switch (opt) {
            case 'c':
                modeComparative = true;
                break;
            case 'p':
                modeCompetition = true;
                break;
            case 'm':
                mapFile = optarg;
                break;
            case 'g':
                managerSo = optarg;
                break;
            case 'a': {
                // split comma-separated list into algoSos
                std::string list(optarg);
                size_t pos = 0;
                while ((pos = list.find(',')) != std::string::npos) {
                    algoSos.push_back(list.substr(0, pos));
                    list.erase(0, pos + 1);
                }
                if (!list.empty()) {
                    algoSos.push_back(list);
                }
                break;
            }
            case 't':
                numThreads = std::stoi(optarg);
                break;
            case 'v':
                verbose = true;
                break;
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

    // Debug print
    if (verbose) {
        std::cerr << "DEBUG: Mode               = "
                  << (modeComparative ? "Comparative" : "Competition") << "\n"
                  << "DEBUG: Map file           = " << mapFile << "\n"
                  << "DEBUG: Manager SO path    = " << managerSo << "\n"
                  << "DEBUG: Algorithm SO paths = ";
        for (const auto& so : algoSos) {
            std::cerr << so << " ";
        }
        std::cerr << "\nDEBUG: Threads            = " << numThreads << "\n";
    }

    // Placeholder: dynamic load manager and algorithms
    // TODO: dlopen(managerSo.c_str(), RTLD_NOW);
    //       GameManagerRegistrar::get().createManagerEntry(...)
    //       validateLastRegistration / removeLast on failure
    //
    // TODO: for each algoSo in algoSos:
    //         AlgorithmRegistrar::get().createAlgorithmFactoryEntry(...)
    //         dlopen(algoSo.c_str(), RTLD_NOW);
    //         validateLastRegistration / removeLast on failure

    std::cout << "[Simulator] CLI parsed successfully.\n";
    return 0;
}
