#include "Tile.h"

Tile::Tile(Point  center,
           float  radius,
           size_t index)
    : _center    ( std::move(center) )
    , _radius    ( radius )
    , _index     ( index )
    , _isOccupied( false )
{}

Point Tile::getCenter() const
{
    return _center;
}

bool Tile::doesContain(Point point) const
{
    /// x,y: point in (C-R; C+R)
    return (point.first  < _center.first  + _radius && point.first  > _center.first  - _radius &&
            point.second < _center.second + _radius && point.second > _center.second - _radius);
}

bool Tile::operator ==(const Tile& other)
{
    return _center == other._center && _radius == other._radius;
}

bool Tile::operator !=(const Tile& other)
{
    return !(*this == other);
}

size_t Tile::getIndex() const
{
    return _index;
}

bool Tile::getIsOccupied() const
{
    return _isOccupied;
}

void Tile::setIsOccupied(bool isOccupied)
{
    _isOccupied = isOccupied;
}
