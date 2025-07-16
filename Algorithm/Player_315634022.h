#pragma once

#include <Player.h>
namespace Algorithm_315634022 {

class Player_315634022 : public Player {
public:
    Player_315634022(int player_idx, size_t x, size_t y, size_t max_steps, size_t num_shells);
    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& view) override;
};
}