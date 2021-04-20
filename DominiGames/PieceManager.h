#pragma once

#include "Piece.h"

class PieceManager
{
public:
    void addPiece(Piece&& piece);

    [[nodiscard]] std::vector<Piece>& getPieces();

    bool checkCollision(Piece*, float, float);
    static bool checkBounds(Piece*, float, float);
    bool tryMove(Piece*, float, float);
    bool canBeSelected(Piece*);

    void select(Piece*);
    bool performMove(Piece*, MoveType);

private:
    std::vector<Piece> _pieces;
    Piece*             _lastSelectedWhitePawn;
    Piece*             _lastSelectedBlackPawn;
};
