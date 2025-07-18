#pragma once

#include <TankAlgorithm.h>

namespace Algorithm_315634022 {

class TankAlgorithm_315634022 : public TankAlgorithm {
public:
    TankAlgorithm_315634022(int, int);
    ActionRequest getAction() override;
    void updateBattleInfo(BattleInfo&) override;


    private:
        MyBattleInfo lastInfo_;
        int          direction_;
        int          shellsLeft_;
        bool         needView_;

        static constexpr int DX[8] = { 0,+1,+1,+1, 0,-1,-1,-1 };
        static constexpr int DY[8] = {-1,-1, 0,+1,+1,+1, 0,-1 };
    };

}

