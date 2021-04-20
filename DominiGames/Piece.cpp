#include "Piece.h"

Piece::Piece(PieceType type, Shader shader, GLuint texture, float x, float y, const Tile& tile)
    : _type(type), _shader(shader), _texture(texture), _currentTile(tile), _isSelected(false), x(x), y(y)
{
    offsetX = 0.0f;
    offsetY = 0.0f;

    // Create transformations
    glm::mat4 transform(1);
    transform = glm::translate( transform,
                                glm::vec3( x + offsetX, y + offsetY, 0.0f ) );
    _transform = transform;
}

void Piece::updateTransform()
{
    glm::mat4 transform(1);
    transform = glm::translate( transform,
                                glm::vec3( x + offsetX, y + offsetY, 0.0f ) );
    _transform = transform;
}

void Piece::draw()
{
    // Get matrix's uniform location and set matrix
    GLint transformLocation = glGetUniformLocation( _shader.getProgram(), "transform" );
    glUniformMatrix4fv( transformLocation, 1, GL_FALSE, glm::value_ptr( _transform ) );

    glActiveTexture( GL_TEXTURE0 + _type );
    glBindTexture( GL_TEXTURE_2D, _texture );
    glUniform1i( glGetUniformLocation( _shader.getProgram(), "ourTexture1" ), _type );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
}

Tile Piece::getCurrentTile() const
{
    return _currentTile;
}

void Piece::setCurrentTile(Tile tile)
{
    _currentTile = tile;
}

bool Piece::operator ==(const Piece& other)
{
    return _type == other._type && _currentTile == other._currentTile &&
            x == other.x && y == other.y && offsetX == other.offsetX && offsetY == other.offsetY;
}

bool Piece::operator !=(const Piece& other)
{
    return !(*this == other);
}

