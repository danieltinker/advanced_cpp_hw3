// Simulator/GameManagerRegistrar.cpp
#include "GameManagerRegistrar.h"

GameManagerRegistrar GameManagerRegistrar::registrar;

GameManagerRegistrar& GameManagerRegistrar::getGameManagerRegistrar() {
    return registrar;
}
