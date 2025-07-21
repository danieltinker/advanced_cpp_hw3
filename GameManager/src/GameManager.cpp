#include "GameManager.h"
#include "GameState.h"

#include <GameResult.h>
#include <Board.h>
#include <SatelliteView.h>
#include <GameManagerRegistration.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace GameManager_315634022 {

// your plugin’s logical name
static constexpr char const* kGMName = "GameManager_315634022";

GameManager::GameManager(bool verbose)
  : verbose_(verbose)
{ }

GameResult GameManager::run(
    size_t map_width, size_t map_height,
    const SatelliteView& satView,
    std::string map_name,
    size_t max_steps, size_t num_shells,
    Player& player1, std::string name1,
    Player& player2, std::string name2,
    TankAlgorithmFactory factory1,
    TankAlgorithmFactory factory2
) {
    // 0) Prepare actions‑log file only if verbose_
    std::string outFile;
    std::ofstream ofs;
    if (verbose_) {
        // strip path & extension from map_name
        std::string name = map_name;
        auto slash = name.find_last_of("/\\");
        if (slash != std::string::npos) name = name.substr(slash + 1);
        auto dot = name.rfind('.');
        if (dot != std::string::npos)    name = name.substr(0, dot);

        // build timestamp "YYYYMMDD_HHMMSS"
        auto now = std::chrono::system_clock::now();
        auto tt  = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
        localtime_r(&tt, &tm);
        std::ostringstream ts_ss;
        ts_ss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        std::string ts = ts_ss.str();

        // filename: <GMName>_output_<Map>_<Algo1>_vs_<Algo2>_<Timestamp>.txt
        outFile = std::string(kGMName) + "_output_"
                + name + "_" + name1 + "_vs_" + name2 + "_" + ts + ".txt";

        ofs.open(outFile);
        if (!ofs) {
            std::cerr << "Error: cannot open actions log file '"
                      << outFile << "' for writing.\n";
            std::exit(1);
        }
    }

    // 1) Build the board from SatelliteView
    Board board(map_height, map_width, verbose_);
    board.loadFromSatelliteView(satView);

    // 2) Construct the GameState (injection‑style ctor)
    GameState state(
        std::move(board),
        std::move(map_name),
        max_steps,
        num_shells,
        player1, name1,
        player2, name2,
        std::move(factory1),
        std::move(factory2),
        verbose_
    );

    // 3) Game loop
    std::size_t turn = 1;
    while (!state.isGameOver()) {
        std::string actions = state.advanceOneTurn();
        if (verbose_) {
            ofs << actions << "\n";
        }
        ++turn;
    }

    // 4) Final board + result
    std::string resultStr = state.getResultString();
    std::cout << resultStr << "\n";
    if (verbose_) {
        ofs << resultStr << "\n";
        ofs.close();
        std::cout << "Actions logged to: " << outFile << "\n";
    }

    // 5) Assemble GameResult
    GameResult gm;
    gm.rounds = turn - 1;

    if (resultStr.rfind("Player 1 won", 0) == 0) {
        gm.winner = 1;
        gm.reason = GameResult::ALL_TANKS_DEAD;
        auto pos = resultStr.find("with ") + 5;
        auto end = resultStr.find(" tanks", pos);
        gm.remaining_tanks = {
            std::stoul(resultStr.substr(pos, end - pos)), 0
        };
    }
    else if (resultStr.rfind("Player 2 won", 0) == 0) {
        gm.winner = 2;
        gm.reason = GameResult::ALL_TANKS_DEAD;
        auto pos = resultStr.find("with ") + 5;
        auto end = resultStr.find(" tanks", pos);
        gm.remaining_tanks = {
            0, std::stoul(resultStr.substr(pos, end - pos))
        };
    }
    else if (resultStr.rfind("Tie, both players have zero tanks", 0) == 0) {
        gm.winner = 0;
        gm.reason = GameResult::ALL_TANKS_DEAD;
        gm.remaining_tanks = {0, 0};
    }
    else {
        // max‑steps tie
        gm.winner = 0;
        gm.reason = GameResult::MAX_STEPS;
        auto p1pos = resultStr.find("player1 has ") + 12;
        auto p1end = resultStr.find(',', p1pos);
        auto a1 = std::stoul(resultStr.substr(p1pos, p1end - p1pos));
        auto p2pos = resultStr.find("player2 has ") + 12;
        auto a2   = std::stoul(resultStr.substr(p2pos));
        gm.remaining_tanks = {a1, a2};
    }

    // 6) Capture final board via SatelliteView subclass
    struct FinalBoardView : public SatelliteView {
        FinalBoardView(const Board& b) : board_(b) {}
        char getObjectAt(size_t x, size_t y) const override {
            switch (board_.getCell(x, y).content) {
                case CellContent::WALL:  return '#';
                case CellContent::MINE:  return '@';
                case CellContent::TANK1: return '1';
                case CellContent::TANK2: return '2';
                default:                 return ' ';
            }
        }
    private:
        Board board_;
    };
    gm.gameState = std::make_unique<FinalBoardView>(state.getBoard());

    return gm;
}

} // namespace GameManager_315634022

using ::GameManager_315634022::GameManager;
// Register for dynamic loading
REGISTER_GAME_MANAGER(GameManager)
