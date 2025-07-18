// GameManager/GameManager_315634022.cpp

#include "GameManager_315634022.h"
#include <ActionRequest.h>
#include <GameManagerRegistration.h>
#include <iostream>
#include <cassert>

namespace GMNS = ::GameManager_315634022;
using GM   = GMNS::GameManager_315634022;
using Tank = GM::Tank;
using Bullet = GM::Bullet;

namespace GameManager_315634022 {

//------------------------------------------------------------------------------
// CompositeView overlays tanks & bullets onto the static map
//------------------------------------------------------------------------------
class CompositeView : public SatelliteView {
public:
    CompositeView(
        const SatelliteView& base,
        const std::vector<Tank>& tanks,
        const std::vector<Bullet>& bullets,
        size_t w, size_t h
    )
      : base_(base), tanks_(tanks), bullets_(bullets),
        width_(w), height_(h)
    {}

    char getObjectAt(size_t x, size_t y) const override {
        // 1) tank?
        for (int i = 0; i < 2; ++i) {
            if (tanks_[i].alive &&
                tanks_[i].x == int(x) &&
                tanks_[i].y == int(y))
            {
                return char('1' + i);
            }
        }
        // 2) bullet?
        for (auto &b : bullets_) {
            if (b.active && b.x == int(x) && b.y == int(y))
                return '*';
        }
        // 3) static map
        return base_.getObjectAt(x,y);
    }

private:
    const SatelliteView&             base_;
    const std::vector<Tank>&         tanks_;
    const std::vector<Bullet>&       bullets_;
    size_t                           width_, height_;
};

//------------------------------------------------------------------------------
// ctor & debug
//------------------------------------------------------------------------------
GM::GameManager_315634022(bool verbose)
  : verbose_(verbose), map_(nullptr), width_(0), height_(0)
{}

void GM::debug(const std::string& msg) {
    if (verbose_) std::cerr << "[GM] " << msg << "\n";
}

//------------------------------------------------------------------------------
// initialize tanks from the static map
//------------------------------------------------------------------------------
void GM::initTanks(
    size_t /*max_steps*/, size_t num_shells,
    TankAlgorithmFactory fac1,
    TankAlgorithmFactory fac2
) {
    tanks_.clear();
    tanks_.resize(2);

    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            char c = map_->getObjectAt(x,y);
            if (c=='1') { tanks_[0].x = int(x); tanks_[0].y = int(y); }
            if (c=='2') { tanks_[1].x = int(x); tanks_[1].y = int(y); }
        }
    }

    for (int i = 0; i < 2; ++i) {
        tanks_[i].dir    = (i==0 ? GM::E : GM::W);
        tanks_[i].shells = int(num_shells);
        tanks_[i].alive  = true;
        tanks_[i].alg    = (i==0 ? fac1(i,0) : fac2(i,0));
    }

    bullets_.clear();
}

//------------------------------------------------------------------------------
// move bullets one cell
//------------------------------------------------------------------------------
void GM::applyBulletMovement() {
    for (auto &b : bullets_) {
        if (!b.active) continue;
        b.x += GM::DX[b.dir];
        b.y += GM::DY[b.dir];
        if (b.x<0 || b.y<0 || b.x>=int(width_) || b.y>=int(height_))
            b.active = false;
    }
}

//------------------------------------------------------------------------------
// resolve bullet‐tank hits
//------------------------------------------------------------------------------
void GM::resolveCollisions() {
    for (auto &b : bullets_) {
        if (!b.active) continue;
        for (int i = 0; i < 2; ++i) {
            if (!tanks_[i].alive) continue;
            if (b.owner!=i && b.x==tanks_[i].x && b.y==tanks_[i].y) {
                debug("Tank " + std::to_string(i+1) + " was hit");
                tanks_[i].alive = false;
                b.active = false;
            }
        }
    }
}

//------------------------------------------------------------------------------
// have both tanks still alive?
//------------------------------------------------------------------------------
bool GM::oneSideDead() const {
    return !(tanks_[0].alive && tanks_[1].alive);
}

//------------------------------------------------------------------------------
// one full turn: update->action->move->resolve
//------------------------------------------------------------------------------
void GM::advanceOneTurn() {
    CompositeView view(*map_, tanks_, bullets_, width_, height_);

    // 1) player→build info→tank
    for (int i = 0; i < 2; ++i) {
        if (!tanks_[i].alive) continue;
        players_[i]->updateTankWithBattleInfo(*tanks_[i].alg, view);
    }

    // 2) getAction + apply
    for (int i = 0; i < 2; ++i) {
        auto &T = tanks_[i];
        if (!T.alive) continue;
        auto act = T.alg->getAction();
        debug("Tank" + std::to_string(i+1) + " => " + std::to_string(int(act)));

        switch (act) {
          case ActionRequest::MoveForward: {
            int nx = T.x + GM::DX[T.dir];
            int ny = T.y + GM::DY[T.dir];
            if (nx>=0 && ny>=0 && nx<int(width_) && ny<int(height_) &&
                map_->getObjectAt(nx,ny)=='.')
            {
                T.x = nx; T.y = ny;
            }
            break;
          }
          case ActionRequest::RotateLeft90:
            T.dir = GM::Dir8((T.dir + 6) % 8);
            break;
          case ActionRequest::RotateRight90:
            T.dir = GM::Dir8((T.dir + 2) % 8);
            break;
          case ActionRequest::Shoot:
            if (T.shells>0) {
              T.shells--;
              bullets_.push_back({T.x,T.y,T.dir,i,true});
            }
            break;
          default:
            break;
        }
    }

    // 3) bullet movement & collisions
    applyBulletMovement();
    resolveCollisions();
}

//------------------------------------------------------------------------------
// run: init everything, loop until end, then package GameResult
//------------------------------------------------------------------------------
GameResult GM::run(
    size_t map_width, size_t map_height,
    const SatelliteView& map,
    std::string map_name,
    size_t max_steps, size_t num_shells,
    Player& player1, std::string /*name1*/,
    Player& player2, std::string /*name2*/,
    TankAlgorithmFactory fac1,
    TankAlgorithmFactory fac2
) {
    debug("Starting run on \"" + map_name + "\"");
    map_    = &map;
    width_  = map_width;
    height_ = map_height;
    players_[0] = &player1;
    players_[1] = &player2;

    initTanks(max_steps, num_shells, fac1, fac2);

    size_t stepCount = 0;
    for (; stepCount < max_steps; ++stepCount) {
        if (oneSideDead()) break;
        advanceOneTurn();
    }

    GameResult res;
    res.rounds = stepCount;
    bool a1 = tanks_[0].alive;
    bool a2 = tanks_[1].alive;

    // winner
    if      (a1 && !a2) res.winner = 1;
    else if (!a1 && a2) res.winner = 2;
    else                res.winner = 0;

    // reason
    if (!a1 && !a2)      res.reason = GameResult::ALL_TANKS_DEAD;
    else if (stepCount==max_steps) res.reason = GameResult::MAX_STEPS;
    else                  res.reason = GameResult::ZERO_SHELLS;

    // remaining tanks
    res.remaining_tanks = {
        std::size_t(tanks_[0].alive),
        std::size_t(tanks_[1].alive)
    };

    // final dynamic view
    res.gameState = std::make_unique<CompositeView>(
        *map_, tanks_, bullets_, width_, height_
    );

    return res;
}

//------------------------------------------------------------------------------
// registration
//------------------------------------------------------------------------------
REGISTER_GAME_MANAGER(GameManager_315634022)

} // namespace GameManager_315634022
