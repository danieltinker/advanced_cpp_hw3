// #pragma once

// #include <AbstractGameManager.h>

// namespace GameManager_315634022 {

// class GameManager_315634022 : public AbstractGameManager {
// public:
//     explicit GameManager_315634022(bool verbose);
//     GameResult run(
//         size_t map_width, size_t map_height,
//         const SatelliteView& map,
//         std::string map_name,
//         size_t max_steps, size_t num_shells,
//         Player& player1, std::string name1,
//         Player& player2, std::string name2,
//         TankAlgorithmFactory player1_tank_algo_factory,
//         TankAlgorithmFactory player2_tank_algo_factory
//     ) override;
// };

// } // namespace GameManager_315634022
// GameManager/GameManager_315634022.h



// starting reconstruction of GM!
#pragma once

#include <AbstractGameManager.h>
#include <SatelliteView.h>
#include <GameResult.h>
#include <Player.h>
#include <TankAlgorithm.h>

#include <string>
#include <vector>
#include <memory>

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
        TankAlgorithmFactory fac1,
        TankAlgorithmFactory fac2
    ) override;

    // 8‐way directions
    enum Dir8 { N = 0, NE, E, SE, S, SW, W, NW };

    // deltas for moving in each of the 8 directions
    static constexpr int DX[8] = {  0,  1,  1,  1,  0, -1, -1, -1 };
    static constexpr int DY[8] = { -1, -1,  0,  1,  1,  1,  0, -1 };

    // make these public so external code can alias them if desired
    struct Tank {
        int x, y;
        Dir8 dir;
        int shells;
        bool alive;
        std::unique_ptr<TankAlgorithm> alg;
    };

    struct Bullet {
        int x, y;
        Dir8 dir;
        int owner;   // 0 or 1
        bool active;
    };

private:
    bool verbose_;
    std::vector<Tank>   tanks_;
    std::vector<Bullet> bullets_;
    Player*             players_[2];
    const SatelliteView* map_;
    size_t              width_, height_;

    void debug(const std::string& msg);

    void initTanks(
        size_t max_steps, size_t num_shells,
        TankAlgorithmFactory fac1,
        TankAlgorithmFactory fac2
    );
    void applyBulletMovement();
    void resolveCollisions();
    bool oneSideDead() const;
    void advanceOneTurn();
};

} // namespace GameManager_315634022
