#pragma once

#include <utility>

using Point = std::pair<float, float>;

class Tile
{
public:
    Tile(Point  center,
         float  radius,
         size_t index);

    [[nodiscard]] Point  getCenter    ()      const;
    [[nodiscard]] bool   doesContain  (Point) const;
    [[nodiscard]] size_t getIndex     ()      const;
    [[nodiscard]] bool   getIsOccupied()      const;

    void setIsOccupied(bool);

    bool operator ==(const Tile& other);
    bool operator !=(const Tile& other);

private:
    Point  _center;
    float  _radius;
    size_t _index;
    bool   _isOccupied;
};
