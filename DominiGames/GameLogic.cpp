#include "GameLogic.h"

#include <algorithm>

GameLogic::GameLogic(PieceManager& pieceManager)
    : _state(MENU)
{
    std::vector<Piece> pieces = pieceManager.getPieces();
    std::vector<Tile> tiles;
    for (int i = 0; i < pieces.size(); ++i)
    {
        if (i < pieces.size() / 2)
            _tilesOfWhite.push_back(pieces[i].getCurrentTile());
        else
            _tilesOfBlack.push_back(pieces[i].getCurrentTile());
    }
}

bool GameLogic::isGameFinished()
{
    return hasWon() || hasLost();
}

bool GameLogic::hasWon()
{
    /**
     *  0 1 2
     *  8 9 10
     *  16 17 18
     */
    std::vector<int> winPos = {0, 1, 2, 8, 9, 10, 16, 17, 18};
    std::vector<int> numTiles;
    for (auto tile : _tilesOfWhite)
        numTiles.push_back(tile.getIndex());
    std::sort(numTiles.begin(), numTiles.end());
    return numTiles == winPos;
}

bool GameLogic::hasLost()
{
    /**
     *  45 46 47
     *  53 54 55
     *  61 62 63
     */
    std::vector<int> losePos = {45, 46, 47, 53, 54, 55, 61, 62, 63};
    std::vector<int> numTiles;
    for (auto tile : _tilesOfBlack)
        numTiles.push_back(tile.getIndex());
    std::sort(numTiles.begin(), numTiles.end());
    return numTiles == losePos;
}

void GameLogic::updatePieces(PieceManager& pieceManager)
{
    _tilesOfWhite.clear();
    _tilesOfBlack.clear();
    std::vector<Piece> pieces = pieceManager.getPieces();
    std::vector<Tile> tiles;
    for (int i = 0; i < pieces.size(); ++i)
    {
        if (i < pieces.size() / 2)
            _tilesOfWhite.push_back(pieces[i].getCurrentTile());
        else
            _tilesOfBlack.push_back(pieces[i].getCurrentTile());
    }
}

GameState GameLogic::getState() const
{
    return _state;
}

void GameLogic::setState(GameState state)
{
    _state = state;
}
