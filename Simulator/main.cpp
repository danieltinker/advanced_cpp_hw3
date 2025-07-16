#include <iostream>
#include "ArgParser.hpp"
#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"
#include "ThreadPool.hpp"

int main(int argc, char* argv[]) {
    Config cfg;
    if (!parseArguments(argc, argv, cfg)) {
        return 1;
    }

    if (cfg.verbose) {
        std::cerr << "[Simulator] CLI parsed successfully.\n";
    }

    // TODO: Load plugins, spawn ThreadPool tasks, etc., using cfg values:
    //   cfg.modeComparative, cfg.game_map, cfg.game_managers_folder,
    //   cfg.algorithm1, cfg.algorithm2, cfg.numThreads, cfg.verbose
    //
    //   or for competition:
    //   cfg.modeCompetition, cfg.game_maps_folder, cfg.game_manager,
    //   cfg.algorithms_folder, cfg.numThreads, cfg.verbose

    std::cout << "[Simulator] Ready to run in "
              << (cfg.modeComparative ? "Comparative" : "Competition")
              << " mode.\n";
    return 0;
}
