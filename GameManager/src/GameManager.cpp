#include "GameManager.h"
#include "GameState.h"
#include <common/GameResult.h>
#include <common/Board.h>
#include <common/SatelliteView.h>

#include <fstream>
#include <iostream>

namespace GameManager_315634022 {

GameManager::GameManager(bool verbose)
  : verbose_(verbose)
{}

// The factory‐injected PlayerFactory/TankAlgorithmFactory should have already
// been stored in members and used to construct game_state_ in your header.

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

    // 1) Build the board from SatelliteView, then initialize GameState
    Board board(map_width, map_height);
    board.loadFromSatelliteView(satView);

    game_state_.initialize(board, max_steps, num_shells);

    // 2) Print initial board
    std::cout << "=== Start Position ===\n";
    game_state_.printBoard();

    // 3) Game loop
    std::size_t turn = 1;
    while (!game_state_.isGameOver()) {
        // a) Print turn header + board
        std::cout << "=== Turn " << turn << " ===\n";
        // b) Advance one full turn, capture the string of actions
        std::string actions = game_state_.advanceOneTurn();
        //    then print the updated board
        game_state_.printBoard();
        // c) Log that turn’s actions
        ofs << actions << "\n";
        ++turn;
    }

    // 4) Final board + result
    std::cout << "=== Final Board ===\n";
    game_state_.printBoard();
    std::string result = game_state_.getResultString();
    std::cout << result << "\n";
    ofs << result << "\n";

    ofs.close();
    std::cout << "Actions logged to: " << outFile << "\n";

    // 5) Assemble and return GameResult
    GameResult gm;
    gm.rounds = turn - 1;

    // Grab the textual result to decide winner & reason:
    std::string result = game_state_.getResultString();

    if (result.rfind("Player 1 won", 0) == 0) {
        gm.winner = 1;
        gm.reason = GameResult::ALL_TANKS_DEAD;
        // parse “with X tanks still alive”
        auto pos = result.find("with ") + 5;
        auto end = result.find(" tanks", pos);
        gm.remaining_tanks = {
            std::stoul(result.substr(pos, end - pos)), 
            0
        };
    }
    else if (result.rfind("Player 2 won", 0) == 0) {
        gm.winner = 2;
        gm.reason = GameResult::ALL_TANKS_DEAD;
        auto pos = result.find("with ") + 5;
        auto end = result.find(" tanks", pos);
        gm.remaining_tanks = {
            0,
            std::stoul(result.substr(pos, end - pos))
        };
    }
    else if (result.rfind("Tie, both players have zero tanks", 0) == 0) {
        gm.winner = 0;
        gm.reason = GameResult::ALL_TANKS_DEAD;
        gm.remaining_tanks = {0, 0};
    }
    else {  // max‐steps tie
        gm.winner = 0;
        gm.reason = GameResult::MAX_STEPS;
        // parse “player1 has A, player2 has B”
        auto p1pos = result.find("player1 has ") + 12;
        auto p1end = result.find(',', p1pos);
        auto a1 = std::stoul(result.substr(p1pos, p1end - p1pos));
        auto p2pos = result.find("player2 has ") + 12;
        auto a2   = std::stoul(result.substr(p2pos));
        gm.remaining_tanks = {a1, a2};
    }

    // Capture the final board as a SatelliteView for the Simulator:
    struct FinalBoardView : public common::SatelliteView {
        FinalBoardView(const Board& b)
          : board_(b) {}
        char getObjectAt(std::size_t x, std::size_t y) const override {
            auto c = board_.getCell(x, y).content;
            switch (c) {
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
    gm.gameState = std::make_unique<FinalBoardView>(board);

    return gm;
    return gm;
}

} // namespace GameManager_315634022

// Register for dynamic loading:
#include <common/GameManagerRegistration.h>
REGISTER_GAME_MANAGER(GameManager_315634022::GameManager)
