#pragma once

#include "Piece.h"

class AI
{
public:
    static int generateSelect();
    static MoveType generateMove();

    void setTilesStatus(std::vector<Tile>&);
    void updatePieces(std::vector<Piece>&);

    std::pair<int, MoveType> move();

private:
    std::vector<std::pair<std::pair<int, int>, int>> _piecesPosition;
    std::vector<int>   _tilesStatus;

    std::pair<int, MoveType> moveSinglePiece(int x, int y);
};
