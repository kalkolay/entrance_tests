#pragma once

#include <map>
#include <vector>

// Other Libs
#include "SOIL2.h"

#include "PieceManager.h"
#include "Tile.h"
#include "AI.h"
#include "GameLogic.h"
#include "Menu.h"
#include "Font.h"

class GameWindow
{
public:
    GameWindow(int, int, const char*, const char*);

    bool init();
    void loadShaders();
    void createBuffers();
    void loadTexture(const char*);

    void render();

    void destroy();

    int moveCount;

private:
    int _width, _height;
    GLFWwindow*         _window;
    GLuint VBO, VAO, EBO, BoardVBO, BoardVAO;
    Shader _pawnShader, _boardShader;
    std::vector<GLuint> _textures;
    std::vector<Tile>   _tiles;
    PieceManager pieces;
    AI computer;
    Menu menu;
    GameLogic logic;
    Font font;

    static void move(GLFWwindow* window, MoveType direction)
    {
        auto* game = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));

        if (game->moveCount % 2 && !game->logic.isGameFinished())
            for (int i = 0; i < 9; ++i)
                if (game->pieces.getPieces()[i].getIsSelected())
                    if (game->pieces.performMove(&game->pieces.getPieces()[i], direction))
                    {
                        int tileIdx = -1;
                        for (int j = 0; j < 64; ++j)
                            if (game->pieces.getPieces()[i].getCurrentTile() == game->_tiles[j])
                            {
                                tileIdx = j;
                                break;
                            }
                        switch (direction)
                        {
                            case UP:
                                game->pieces.getPieces()[i].setCurrentTile(game->_tiles[tileIdx - 8]);
                                game->_tiles[tileIdx - 8].setIsOccupied(true);
                                break;
                            case LEFT:
                                game-> pieces.getPieces()[i].setCurrentTile(game->_tiles[tileIdx - 1]);
                                game->_tiles[tileIdx - 1].setIsOccupied(true);
                                break;
                            case DOWN:
                                game->pieces.getPieces()[i].setCurrentTile(game->_tiles[tileIdx + 8]);
                                game->_tiles[tileIdx + 8].setIsOccupied(true);
                                break;
                            case RIGHT:
                                game->pieces.getPieces()[i].setCurrentTile(game->_tiles[tileIdx + 1]);
                                game->_tiles[tileIdx + 1].setIsOccupied(true);
                                break;
                            default:
                                break;
                        }
                        game->_tiles[tileIdx].setIsOccupied(false);
                        ++game->moveCount;
                    }
    }


    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            switch (key)
            {
                case GLFW_KEY_W:
                case GLFW_KEY_UP:
                    move(window, UP);
                    break;
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT:
                    move(window, LEFT);
                    break;
                case GLFW_KEY_S:
                case GLFW_KEY_DOWN:
                    move(window, DOWN);
                    break;
                case GLFW_KEY_D:
                case GLFW_KEY_RIGHT:
                    move(window, RIGHT);
                    break;
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
                default:
                    break;
            }
        }
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto* game = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT && !game->logic.isGameFinished())
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            double normalizedX = -1.0 + 2.0 * xpos / game->_width;
            double normalizedY = 1.0 - 2.0 * ypos / game->_height;

            if (game->logic.getState() == PLAY)
            {
                for (auto tile : game->_tiles)
                    if (tile.doesContain({normalizedX, normalizedY}))
                        for (int i = 0; i < 9; ++i)
                            if (game->pieces.getPieces()[i].getCurrentTile() == tile)
                            {
                                game->pieces.select(&game->pieces.getPieces()[i]);
                                break;
                            }
            }
            else if (game->logic.getState() == MENU || game->logic.getState() == INFO)
            {
                for (auto tile : game->menu.getTiles())
                    if (tile.doesContain({normalizedX, normalizedY}))
                    {
                        switch (tile.getIndex())
                        {
                            case 0:    // play
                                game->logic.setState(PLAY);
                                break;
                            case 1:    // info
                                game->logic.setState(INFO);
                                break;
                            case 2:    // exit
                                glfwSetWindowShouldClose(window, true);
                                break;
                            default:   // return
                                game->logic.setState(MENU);
                                break;
                        }
                    }
            }
        }
    }
};
