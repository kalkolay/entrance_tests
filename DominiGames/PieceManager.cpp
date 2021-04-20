#include "PieceManager.h"

bool PieceManager::checkCollision(Piece* piece, float x, float y)
{
    for (int j = 0; j < 18; ++j)
    {
        float xpos = piece->getPosition().first + x;
        float ypos = piece->getPosition().second + y;
        if (*piece != _pieces[j] &&
            _pieces[j].getCurrentTile().getCenter().first == xpos &&
            _pieces[j].getCurrentTile().getCenter().second == ypos)
            return false;
    }
    return true;
}

void PieceManager::addPiece(Piece&& piece)
{
    _pieces.push_back(piece);
}

std::vector<Piece>& PieceManager::getPieces()
{
    return _pieces;
}

bool PieceManager::checkBounds(Piece* piece, float x, float y)
{
    float tmpX = piece->getPosition().first + x, tmpY = piece->getPosition().second + y;
    return tmpX >= -0.875f && tmpX <= 0.875f && tmpY >= -0.875f && tmpY <= 0.875f;
}

bool PieceManager::tryMove(Piece* piece, float x, float y)
{
    return checkBounds(piece, x, y) && checkCollision(piece, x, y);
}

bool PieceManager::canBeSelected(Piece* piece)
{
    return tryMove(piece, 0.0f, 0.25f) || tryMove(piece, -0.25f, 0.0f) ||
           tryMove(piece, 0.0f, -0.25f) || tryMove(piece, 0.25f, 0.0f);
}

void PieceManager::select(Piece* piece)
{
    if (canBeSelected(piece))
    {
        if (piece->getType() == WHITE)
        {
            if (_lastSelectedWhitePawn != nullptr)
                _lastSelectedWhitePawn->setIsSelected(false);
            _lastSelectedWhitePawn = piece;
        }
        else  // BLACK
        {
            if (_lastSelectedBlackPawn != nullptr)
                _lastSelectedBlackPawn->setIsSelected(false);
            _lastSelectedBlackPawn = piece;
        }
        piece->setIsSelected(true);
    }
}

bool PieceManager::performMove(Piece* piece, MoveType direction)
{
    float x, y;
    switch (direction)
    {
        case UP:
            x = 0.0f;
            y = 0.25f;
            break;
        case LEFT:
            x = -0.25f;
            y = 0.0f;
            break;
        case DOWN:
            x = 0.0f;
            y = -0.25f;
            break;
        default:    // RIGHT
            x = 0.25f;
            y = 0.0f;
            break;
    }

    if (tryMove(piece, x, y))
    {
        piece->setOffset(x, y);
        piece->setIsSelected(false);
        return true;
    }

    return false;
}
