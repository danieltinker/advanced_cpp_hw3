#include <Player.h>
#include <PlayerRegistration.h>

namespace AlgorithmAlt_315634022 {

class PlayerAlt_315634022 : public Player {
public:
    PlayerAlt_315634022(int, size_t, size_t, size_t, size_t) {}
    void updateTankWithBattleInfo(TankAlgorithm&, SatelliteView&) override {}
};

REGISTER_PLAYER(PlayerAlt_315634022);

} // namespace AlgorithmAlt_315634022
