// src/Board.cpp
#include "Board.h"

Board::Board(std::size_t rows, std::size_t cols)
  : rows_(rows), cols_(cols),
    grid_(rows, std::vector<Cell>(cols))
{}

void Board::setCell(int x, int y, CellContent c) {
    Cell& cell = grid_[y][x];
    cell.content         = c;
    cell.wallHits        = (c == CellContent::WALL ? 0 : 0);
    cell.hasShellOverlay = false;
}

void Board::wrapCoords(int& x, int& y) const {
    int w = int(cols_), h = int(rows_);
    x = (x % w + w) % w;
    y = (y % h + h) % h;
}

void Board::clearShellMarks() {
    for (auto& row : grid_)
        for (auto& cell : row)
            cell.hasShellOverlay = false;
}

void Board::loadFromSatelliteView(const SatelliteView& view) {
    // assume rows_ and cols_ were set by your ctor
    for (std::size_t y = 0; y < rows_; ++y) {
        for (std::size_t x = 0; x < cols_; ++x) {
            char obj = view.getObjectAt(x, y);
            switch (obj) {
                case '#': setCell(x, y, CellContent::WALL);    break;
                case '@': setCell(x, y, CellContent::MINE);    break;
                case '1': setCell(x, y, CellContent::TANK1);   break;
                case '2': setCell(x, y, CellContent::TANK2);   break;
                default : setCell(x, y, CellContent::EMPTY);  break;
            }
        }
    }
}