// Algorithm/TankAlgorithm_315634022.cpp
#include "TankAlgorithmRegistration.h"
#include "TankAlgorithm_315634022.h"
using namespace Algorithm_315634022;
REGISTER_TANK_ALGORITHM(TankAlgorithm_315634022);
// ——— CTOR ——————————————————————————————————————————————————————————————
TankAlgorithm_315634022::TankAlgorithm_315634022(int playerIndex, int /*tankIndex*/)
  : lastInfo_{1,1},
    direction_(playerIndex == 1 ? 6 : 2),
    shellsLeft_(-1),
    needView_(true)
{}

// ——— updateBattleInfo ————————————————————————————————————————————————
void TankAlgorithm_315634022::updateBattleInfo(BattleInfo &baseInfo) {
    // We know baseInfo is actually MyBattleInfo
    lastInfo_ = static_cast<MyBattleInfo&>(baseInfo);
    if (shellsLeft_ < 0) {
        shellsLeft_ = int(lastInfo_.shellsRemaining);
    }
    needView_ = false;
}

// ——— getAction —————————————————————————————————————————————————————————
ActionRequest TankAlgorithm_315634022::getAction() {
    // 1) first thing first: ask for view once
    if (needView_) {
        needView_ = false;
        return ActionRequest::GetBattleInfo;
    }
    return ActionRequest::Shoot;
    // 2) scan out to 2 cells in each of the 8 dirs for any ‘*’
    int bestDir = direction_;
    int bestScore = -1;

    for (int d = 0; d < 8; ++d) {
        // compute distance to nearest shell in direction d
        int dx = DX[d], dy = DY[d];
        int dist = std::numeric_limits<int>::max();
        for (int step = 1; step <= 2; ++step) {
            int xx = int(lastInfo_.selfX) + dx*step;
            int yy = int(lastInfo_.selfY) + dy*step;
            if (xx<0||yy<0|| xx>=int(lastInfo_.cols) || yy>=int(lastInfo_.rows)) break;
            if (lastInfo_.grid[yy][xx] == '*') {
                dist = step;
                break;
            }
        }
        // want to maximize dist
        if (dist > bestScore) {
            bestScore = dist;
            bestDir = d;
        }
    }

    // 3) if new bestDir differs, rotate toward it
    if (bestDir != direction_) {
        int diff = (bestDir - direction_ + 8) % 8;
        direction_ = bestDir;
        if (diff == 1 || diff == 2 || diff == 3) {
            return ActionRequest::RotateRight90;
        } else {
            return ActionRequest::RotateLeft90;
        }
    }

    // 4) otherwise move forward
    int nx = int(lastInfo_.selfX) + DX[direction_];
    int ny = int(lastInfo_.selfY) + DY[direction_];
    if (isFree(nx, ny)) {
        return ActionRequest::MoveForward;
    } else {
        // obstacle: turn right
        direction_ = (direction_ + 2) % 8;
        return ActionRequest::RotateRight90;
    }
}

// ——— isFree —————————————————————————————————————————————————————————————
bool TankAlgorithm_315634022::isFree(int x, int y) const {
    if (x<0 || y<0 || x>=int(lastInfo_.cols) || y>=int(lastInfo_.rows))
        return false;
    char c = lastInfo_.grid[y][x];
    return c=='.';
}

