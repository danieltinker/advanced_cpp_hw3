#include <TankAlgorithm.h>
#include <TankAlgorithmRegistration.h>

namespace AlgorithmAlt_315634022 {

class TankAlgorithmAlt_315634022 : public TankAlgorithm {
public:
    TankAlgorithmAlt_315634022(int, int) {}
    ActionRequest getAction() override {
        return ActionRequest::MoveForward;  // different behavior stub
    }
    void updateBattleInfo(BattleInfo&) override {}
};
REGISTER_TANK_ALGORITHM(TankAlgorithmAlt_315634022)

} // namespace AlgorithmAlt_315634022
