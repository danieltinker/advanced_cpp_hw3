#pragma once

#include <TankAlgorithm.h>

namespace Algorithm_315634022 {

class TankAlgorithm_315634022 : public TankAlgorithm {
public:
    TankAlgorithm_315634022(int, int);
    ActionRequest getAction() override;
    void updateBattleInfo(BattleInfo&) override;
};

}