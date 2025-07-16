#pragma once

#include <AbstractGameManager.h>

namespace GameManager_315634022 {

class GameManager_315634022 : public AbstractGameManager {
public:
    explicit GameManager_315634022(bool verbose);
    GameResult run(
        size_t map_width, size_t map_height,
        const SatelliteView& map,
        std::string map_name,
        size_t max_steps, size_t num_shells,
        Player& player1, std::string name1,
        Player& player2, std::string name2,
        TankAlgorithmFactory player1_tank_algo_factory,
        TankAlgorithmFactory player2_tank_algo_factory
    ) override;
};

} // namespace GameManager_315634022
