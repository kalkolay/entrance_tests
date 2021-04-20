#include "AI.h"

#include "LeeAlgorithm.h"

#include <random>

int AI::generateSelect()
{
    std::random_device rd;
    std::uniform_int_distribution<> uid(9, 17);
    return uid(rd);
}

MoveType AI::generateMove()
{
    std::random_device rd;
    std::uniform_int_distribution<> uid(0, 3);
    int direction = uid(rd);
    return (MoveType)direction;
}

void AI::setTilesStatus(std::vector<Tile>& tiles)
{
    std::vector<int> tmp;
    for (int idx = 0; idx < 64; ++idx)
        tmp.push_back(tiles[idx].getIsOccupied());
    _tilesStatus = tmp;
}

void AI::updatePieces(std::vector<Piece>& pieces)
{
    std::vector<std::pair<std::pair<int, int>, int>> tmp;
    for (int i = 0; i < pieces.size(); ++i)
        if (pieces[i].getType() == BLACK)
        {
            std::pair<int, int> val = {pieces[i].getCurrentTile().getIndex() / 8,
                                       pieces[i].getCurrentTile().getIndex() % 8};
            tmp.emplace_back(val, i);
        }
    _piecesPosition = tmp;
}

std::pair<int, MoveType> AI::moveSinglePiece(int x, int y)
{
    std::vector<Pos> goal =
        {
            {5, 5}, {5, 6}, {5, 7},
            {6, 5}, {6, 6}, {6, 7},
            {7, 5}, {7, 6}, {7, 7}
        };
    LeeAlgorithm pat;
    std::vector<std::pair<int, Pos>> res;

    for (auto & i : goal)
    {
        pat.in(_tilesStatus, {x, y}, i);
        pat.find_start_end();
        if ( pat.lee() )
            res.push_back( pat.out() );
    }

    if (res.empty())  // no valid moves
    {
        return {0, NONE};
    }
    std::sort(res.begin(), res.end(),
              [](std::pair<int, Pos> x, std::pair<int, Pos> y){ return x.first < y.first; });

    MoveType ans = NONE;
    Pos direction = {x - res[0].second.x, y - res[0].second.y};
    if (direction.x == 0 && direction.y == -1)
        ans = RIGHT;
    else if (direction.x == 0 && direction.y == 1)
        ans = LEFT;
    else if (direction.x == -1 && direction.y == 0)
        ans = DOWN;
    else if (direction.x == -1 && direction.y == 0)
        ans = UP;

    return {res[0].first, ans};
}

std::pair<int, MoveType> AI::move()
{
    // длина, куда, индекс пешки
    using ResType = std::pair<int, std::pair<MoveType, int>>;
    std::vector<ResType> res;

    for (auto pawn : _piecesPosition)
    {
        int len = moveSinglePiece(pawn.first.first, pawn.first.second).first;
        MoveType pos = moveSinglePiece(pawn.first.first, pawn.first.second).second;
        int index = pawn.second;
        if (len != 0 && pos != NONE)
            res.push_back({len, {pos, index}});
    }

    std::sort(res.begin(), res.end(),
              [](ResType x, ResType y) {
                  return x.first < y.first;
              });
    ///std::cout << "Result: pawn " << res[0].second.second << ": (" << res[0].second.first << ")\n";
    int pieceIndex;
    MoveType moveType;
    // first: int (length)
    // second: std::pair<std::pair<int, int>, int> (pos, index)
    if (res.empty() || res[0].first == 0 || res[0].second.first == NONE)
    {
        pieceIndex = generateSelect();
        moveType = generateMove();
    }
    else
    {
        pieceIndex = res[0].second.second;
        moveType = res[0].second.first;
    }
    return {pieceIndex, moveType};
}
