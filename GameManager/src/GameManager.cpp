#include "GameManager.h"
#include "GameState.h"

#include <GameResult.h>
#include <Board.h>
#include <SatelliteView.h>
#include <GameManagerRegistration.h>

#include <fstream>
#include <iostream>
#include <memory>

namespace GameManager_315634022 {

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
    // 0) Derive output filename
    std::string name = map_name;
    auto slash = name.find_last_of("/\\");
    if (slash != std::string::npos) name = name.substr(slash + 1);
    auto dot = name.rfind('.');
    if (dot != std::string::npos) name = name.substr(0, dot);
    std::string outFile = "output_" + name + ".txt";

    std::ofstream ofs(outFile);
    if (!ofs) {
        std::cerr << "Error: cannot open actions log file '"
                  << outFile << "' for writing.\n";
        std::exit(1);
    }

    // 1) Build the board from SatelliteView
    Board board(map_width, map_height,verbose_);
    board.loadFromSatelliteView(satView);

    // 2) Construct the GameState (injection‐style ctor)
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

    // 3) Print initial board
    std::cout << "=== Start Position ===\n";
    state.printBoard();

    // 4) Game loop
    std::size_t turn = 1;
    while (!state.isGameOver()) {
        std::cout << "=== Turn " << turn << " ===\n";
        std::string actions = state.advanceOneTurn();
        state.printBoard();
        ofs << actions << "\n";
        ++turn;
    }

    // 5) Final board + result
    std::cout << "=== Final Board ===\n";
    state.printBoard();
    std::string resultStr = state.getResultString();
    std::cout << resultStr << "\n";
    ofs << resultStr << "\n";
    ofs.close();
    std::cout << "Actions logged to: " << outFile << "\n";

    // 6) Assemble GameResult
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
        // max‐steps tie
        gm.winner = 0;
        gm.reason = GameResult::MAX_STEPS;
        auto p1pos = resultStr.find("player1 has ") + 12;
        auto p1end = resultStr.find(',', p1pos);
        auto a1 = std::stoul(resultStr.substr(p1pos, p1end - p1pos));
        auto p2pos = resultStr.find("player2 has ") + 12;
        auto a2   = std::stoul(resultStr.substr(p2pos));
        gm.remaining_tanks = {a1, a2};
    }

    // 7) Capture final board via SatelliteView subclass
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
using ::GameManager_315634022::GameManager;
// Register for dynamic loading
REGISTER_GAME_MANAGER(GameManager)
} // namespace GameManager_315634022

