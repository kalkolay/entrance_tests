#include <iostream>

#include "Piece.h"
#include "GameWindow.h"

// Window dimensions
const GLuint WIDTH = 750, HEIGHT = 750;
const GLchar* vPath = "shaders/core.vert", *fPath = "shaders/core.frag";


int main( )
{
    GameWindow window(WIDTH, HEIGHT, vPath, fPath);
    if ( !window.init() )
        return EXIT_FAILURE;

    window.loadShaders();

    window.createBuffers();

    window.loadTexture("images/pawn-white.png");
    window.loadTexture("images/pawn-black.png");
    window.loadTexture("images/chessboard.png");

    window.render();

    window.destroy();

    return EXIT_SUCCESS;
}
