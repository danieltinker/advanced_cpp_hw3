// Algorithm/EvasiveTank.h
#pragma once
#include "TankAlgorithm.h"
#include "MyBattleInfo.h"
#include "ActionRequest.h"
#include <queue>
#include <limits>

/// A “stay clear of shells” tank.
/// See ArenaBattle/EvasiveTank for a full description.
class EvasiveTank : public TankAlgorithm {
public:
    EvasiveTank(int playerIndex, int /*tankIndex*/);

    // Called once per turn _before_ getAction() if you returned GetBattleInfo.
    void updateBattleInfo(BattleInfo &baseInfo) override;

    // Called once per turn to ask “what do I do now?”
    ActionRequest getAction() override;

private:
    MyBattleInfo lastInfo_;
    int          direction_;
    int          shellsLeft_;
    bool         needView_;

    static constexpr int DX[8] = { 0,+1,+1,+1, 0,-1,-1,-1 };
    static constexpr int DY[8] = {-1,-1, 0,+1,+1,+1, 0,-1 };

    /// Helper: within bounds and not a wall (‘#’), mine (‘@’) or tank (‘1’/‘2’).
    bool isFree(int x, int y) const;
};
