// Simulator/main.cpp

#include <iostream>     // std::cout, std::cerr
#include <vector>       // std::vector
#include <string>       // std::string
#include <dlfcn.h>      // dlopen, dlerror
#include <cassert>      // assert

#include "AlgorithmRegistrar.h"

int main() {
    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    std::vector<std::string> names = {"Algorithm_315634022", "Bad"};
    for (const auto& name : names) {
        registrar.createAlgorithmFactoryEntry(name);

        // open the plugin
        void* handle = dlopen("../Algorithm/libAlgorithm_315634022.so",
                              RTLD_LAZY | RTLD_GLOBAL);
        if (!handle) {
            std::cerr << "dlopen failed: " << dlerror() << "\n";
            return 1;
        }

        try {
            registrar.validateLastRegistration();
            std::cout <<"Added an algorithm succesfully."<<std::endl;
        }
        catch (AlgorithmRegistrar::BadRegistrationException& e) {
            std::cout <<"caught a bad one"<<std::endl;
            registrar.removeLast();
        }
    }

    // exercise all registered algorithms
    for (const auto& algo : registrar) {
        auto algorithm = algo.createTankAlgorithm(1, 0);
        std::cout << algo.name() << ": "
                  << static_cast<int>(algorithm->getAction())
                  << std::endl;
    }

    registrar.clear();
    // TODO: dlclose handles if needed
    return 0;
}
