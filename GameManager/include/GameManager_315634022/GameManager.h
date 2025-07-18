#pragma once
#include <common/AbstractGameManager.h>

namespace GameManager_315634022 {

class GameManager : public AbstractGameManager {
public:
    explicit GameManager(bool verbose);
    ~GameManager() override = default;

    // Implements the core loop you already had in ArenaBattleFinal/src/GameManager.cpp
    GameResult run(
        size_t map_width,
        size_t map_height,
        const SatelliteView& map,
        std::string map_name,
        size_t max_steps,
        size_t num_shells,
        Player& player1, std::string name1,
        Player& player2, std::string name2,
        TankAlgorithmFactory player1_tank_algo_factory,
        TankAlgorithmFactory player2_tank_algo_factory
    ) override;

private:
    bool verbose_;
};

} // namespace GameManager_<YOUR_ID>
