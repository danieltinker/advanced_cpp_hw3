#include "FinalBoardView.h"
using namespace GameManager_315634022;
FinalBoardView::FinalBoardView(const Board& finalBoard)
  : board_(finalBoard)
{}

GameResult FinalBoardView::toResult() const {
    // e.g. serialize your board into scores, positions, etc.
    // Here’s a sketch—fill in with your actual logic:
    GameResult result;
    // result.mapName   = board_.name();
    // result.winner    = board_.determineWinner();
    // result.moveCount = board_.stepCount();
    // ...any other fields...
    return result;
}
