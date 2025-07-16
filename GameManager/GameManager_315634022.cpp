#include "GameManager_315634022.h"
#include "../UserCommon/Log.h"

// bring in the macro—you must locate the right header under your common/ folder,
// e.g. common/GameManagerRegistration.h or common/registration/GameManagerRegistration.h
#include <GameManagerRegistration.h>

namespace GameManager_315634022 {

  // 1) Define your constructor
  GameManager_315634022::GameManager_315634022(bool verbose) {
    UserCommon_315634022::debug("[GM] constructed, verbose=" 
      + std::to_string(verbose));
  }

  // 2) Define the override of the 12-parameter run() exactly as in AbstractGameManager
  GameResult GameManager_315634022::run(
      size_t map_width,
      size_t map_height,
      const SatelliteView& map,
      std::string map_name,
      size_t max_steps,
      size_t num_shells,
      Player& player1,
      std::string name1,
      Player& player2,
      std::string name2,
      TankAlgorithmFactory f1,
      TankAlgorithmFactory f2
  ) {
    UserCommon_315634022::debug(
      "[GM] run “" + map_name + "” for " + std::to_string(max_steps) + " steps"
    );
    GameResult res;
    res.winner = 0;
    res.reason = GameResult::Reason::MAX_STEPS;
    // res.rounds = 0;
    return res;
  }

  // 3) Register your class so the simulator loader can find it
  REGISTER_GAME_MANAGER(GameManager_315634022)

} // namespace GameManager_315634022
