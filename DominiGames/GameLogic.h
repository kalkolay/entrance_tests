#pragma once

#include "PieceManager.h"
#include "Tile.h"

enum GameState
{
    MENU,
    PLAY,
    INFO
};

class GameLogic
{
public:
    explicit GameLogic(PieceManager&);

    void updatePieces(PieceManager&);

    bool isGameFinished();
    bool hasWon();
    bool hasLost();

    [[nodiscard]] GameState getState() const;
    void setState(GameState state);

private:
    std::vector<Tile> _tilesOfWhite;
    std::vector<Tile> _tilesOfBlack;
    GameState         _state;
};