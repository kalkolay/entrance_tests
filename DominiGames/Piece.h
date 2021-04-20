#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Tile.h"


enum PieceType
{
    WHITE,
    BLACK
};

enum MoveType
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class Piece
{
public:
    Piece(PieceType, Shader, GLuint, float, float, const Tile&);

    [[nodiscard]] PieceType getType() const
    {
        return _type;
    }

    void updateTransform();

    [[nodiscard]] Tile getCurrentTile() const;
    void setCurrentTile(Tile);

    [[nodiscard]] bool getIsSelected() const { return _isSelected; }
    void setIsSelected(bool flag) { _isSelected = flag; }

    [[nodiscard]] std::pair<float, float> getPosition() const
    {
        return {x + offsetX, y + offsetY};
    }

    void setOffset(float xpos, float ypos)
    {
        if (_isSelected || _type == BLACK)
        {
            offsetX += xpos;
            offsetY += ypos;

            updateTransform();
        }
    }

    void draw();

    bool operator ==(const Piece& other);
    bool operator !=(const Piece& other);

private:
    PieceType _type;
    Shader    _shader;
    GLuint    _texture;
    glm::mat4 _transform;
    Tile      _currentTile;
    bool      _isSelected;
    float x, y;
    float offsetX, offsetY;
};
