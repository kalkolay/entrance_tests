#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL2.h"

#include "Shader.h"
#include "Tile.h"

enum MenuItems
{
    PLAY_BUTTON,
    INFO_BUTTON,
    EXIT_BUTTON,
    RETURN_BUTTON
};

class Menu
{
public:
    Menu();

    [[nodiscard]] std::vector<Tile> getTiles() const
    {
        return _tiles;
    }

    void init();
    void loadTexture(const char* tPath);

    void drawItem(MenuItems);

private:
    Shader shader;
    GLuint VBO, VAO, EBO;
    std::vector<GLuint> _textures;
    std::vector<Tile> _tiles;
};
