#include "GameWindow.h"

GameWindow::GameWindow(int width, int height, const char* vSource, const char* fSource)
    : _width(width)
    , _height(height)
    , _pawnShader(vSource, fSource)
    , _boardShader(_pawnShader)
    , font(width, height)
    , logic(pieces)
    , moveCount(1)
{
    int index = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            Point center = {-1.0f + (2 * j + 1) * 0.125f,
                            1.0f - (2 * i + 1) * 0.125f};
            Tile tile(center, 0.125f, index);
            _tiles.push_back(tile);
            ++index;
        }
    }
}

bool GameWindow::init()
{
    // Init GLFW
    glfwInit( );

    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    // Create a GLFW window object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow( _width, _height,
                                           "DominiGames Test Assignment",
                                           nullptr, nullptr );

    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    if ( nullptr == window )
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );

        return false;
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwMakeContextCurrent( window );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );

    // enable alpha support
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    _window = window;

    menu.init();

    return true;
}

void GameWindow::loadShaders()
{
    _pawnShader.init();
    _boardShader.init();
}

void GameWindow::createBuffers()
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
            {
                    // Positions          // Colors           // Texture Coords
                    0.125f,  0.125f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
                    0.125f, -0.125f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
                    -0.125f, -0.125f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
                    -0.125f,  0.125f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
            };
    GLfloat boardVertices[] =
            {
                    // Positions          // Colors           // Texture Coords
                    1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
                    1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
                    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
                    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
            };
    GLuint indices[] =
            {  // Note that we start from 0!
                    0, 1, 3, // First Triangle
                    1, 2, 3  // Second Triangle
            };
    glGenVertexArrays( 1, &VAO );
    glGenVertexArrays( 1, &BoardVAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &BoardVBO );
    glGenBuffers( 1, &EBO );

    //////////////////////////////

    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STREAM_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STREAM_DRAW );

    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray(1);
    // Texture Coordinate attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 ); // Unbind VAO

    //////////////////////////////

    glBindVertexArray( BoardVAO );

    glBindBuffer( GL_ARRAY_BUFFER, BoardVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( boardVertices ), boardVertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray(1);
    // Texture Coordinate attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 ); // Unbind BoardVAO
}

void GameWindow::loadTexture(const char* tPath)
{
    GLuint texture;

    int width, height;

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    // Set our texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // Set texture filtering
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    unsigned char* image = SOIL_load_image( tPath,
                                             &width, &height, 0, SOIL_LOAD_RGBA );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );

    _textures.push_back(texture);

    menu.loadTexture("images/play-button.png");
    menu.loadTexture("images/info-button.png");
    menu.loadTexture("images/exit-button.png");
    menu.loadTexture("images/return-button.png");
}

void GameWindow::render()
{
    for (auto i = 0; i < 3; ++i)
        for (auto j = 0; j < 3; ++j)
        {
            Tile* tileTemp;
            for (auto tile : _tiles)
                if (tile.getCenter().first == 1.0f - (2 * j + 1) * 0.125f &&
                    tile.getCenter().second == -1.0f + (2 * i + 1) * 0.125f)
                {
                    _tiles[tile.getIndex()].setIsOccupied(true);
                    tileTemp = new Tile(tile);
                    tileTemp->setIsOccupied(true);
                }
            Piece pawnWhite(WHITE, _pawnShader, _textures[0],
                            1.0f - (2 * j + 1) * 0.125f,
                            -1.0f + (2 * i + 1) * 0.125f,
                            *tileTemp);
            pieces.addPiece(std::move(pawnWhite));
        }

    for (auto i = 0; i < 3; ++i)
        for (auto j = 0; j < 3; ++j)
        {
            Tile* tileTemp;
            for (auto tile : _tiles)
                if (tile.getCenter().first == -1.0f + (2 * j + 1) * 0.125f &&
                    tile.getCenter().second == 1.0f - (2 * i + 1) * 0.125f)
                {
                    _tiles[tile.getIndex()].setIsOccupied(true);
                    tileTemp = new Tile(tile);
                    tileTemp->setIsOccupied(true);
                }
            Piece pawnBlack(BLACK, _pawnShader, _textures[1],
                            -1.0f + (2 * j + 1) * 0.125f,
                            1.0f - (2 * i + 1) * 0.125f,
                            *tileTemp);
            //pawnBlack.draw();
            pieces.addPiece(std::move(pawnBlack));
        }


    font.init();
    font.loadBuffers();


    // Game loop
    while ( !glfwWindowShouldClose( _window ) )
    {
        // Check if any events have been activated (key pressed, mouse moved etc.)
        // and call corresponding response functions
        glfwPollEvents( );

        // Render
        // Clear the color buffer
        if (logic.hasWon() || logic.getState() != PLAY)
            glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        else
            glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );


        if (logic.getState() == MENU)
        {
            font.RenderText("DominiGames",
                       _width / 2 - 150, _height / 2 + 150, 1.0f,
                       glm::vec3(1.0f, 0.5f, 0.0f));
            font.RenderText("Test Assignment",
                       _width / 2 - 175, _height / 2 + 90, 1.0f,
                       glm::vec3(1.0f, 0.5f, 0.0f));
            font.RenderText("Author: @kalkolay",
                       540.0f, 30.0f, 0.5f,
                       glm::vec3(0.5f, 0.5f, 0.5f));
            menu.drawItem(PLAY_BUTTON);
            menu.drawItem(INFO_BUTTON);
            menu.drawItem(EXIT_BUTTON);
        }
        else if (logic.getState() == INFO)
        {
            font.RenderText("How to play?",
                       _width / 2 - 125, _height / 2 + 150, 1.0f,
                       glm::vec3(0.0f, 0.0f, 0.0f));
            font.RenderText("Left mouse click - select pawn",
                       _width / 2 - 230, _height / 2 + 50, 0.8f,
                       glm::vec3(0.5f, 0.5f, 0.5f));
            font.RenderText("W, A, S, D (or arrows) - move",
                       _width / 2 - 230, _height / 2, 0.8f,
                       glm::vec3(0.5f, 0.5f, 0.5f));
            font.RenderText("Have fun!",
                       _width / 2 - 90, _height / 2 - 100, 1.0f,
                       glm::vec3(1.0f, 0.5f, 0.0f));
            font.RenderText("Author: @kalkolay",
                       540.0f, 30.0f, 0.5f,
                       glm::vec3(0.5f, 0.5f, 0.5f));

            menu.drawItem(RETURN_BUTTON);
        }
        else if (logic.getState() == PLAY)
        {
            _boardShader.use( );

            glBindVertexArray( BoardVAO );

            glm::mat4 transform(1);
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -1.0f));

            GLint transformLoc = glGetUniformLocation(_boardShader.getProgram(), "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            glActiveTexture( GL_TEXTURE0 + 2 );
            glBindTexture( GL_TEXTURE_2D, _textures[2] );
            glUniform1i( glGetUniformLocation( _boardShader.getProgram(), "ourTexture1" ), 2 );

            if (!logic.isGameFinished())
                glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

            glBindVertexArray( 0 );

            // Draw the triangle
            _pawnShader.use( );

            glBindVertexArray( VAO );

            logic.updatePieces(pieces);

            if (!logic.isGameFinished())
            {
                while (moveCount % 2 == 0)  // perform AI move
                {
                    computer.setTilesStatus(_tiles);
                    computer.updatePieces(pieces.getPieces());
                    std::pair<int, MoveType> res = computer.move();
                    int idx = res.first;
                    MoveType direction = res.second;
                    if (!pieces.performMove(&pieces.getPieces()[idx], direction))
                    {
                        idx = AI::generateSelect();
                        direction = (idx + 1) % 2 == 0 ? RIGHT : DOWN;
                        if (!pieces.performMove(&pieces.getPieces()[idx], direction))
                            continue;
                    }
                    int tileIdx = -1;
                    for (int j = 0; j < 64; ++j)
                    {
                        if (pieces.getPieces()[idx].getCurrentTile() == _tiles[j])
                        {
                            tileIdx = j;
                            break;
                        }
                    }
                    switch (direction)
                    {
                        case UP:
                            pieces.getPieces()[idx].setCurrentTile(_tiles[tileIdx - 8]);
                            _tiles[tileIdx - 8].setIsOccupied(true);
                            break;
                        case LEFT:
                            pieces.getPieces()[idx].setCurrentTile(_tiles[tileIdx - 1]);
                            _tiles[tileIdx - 1].setIsOccupied(true);
                            break;
                        case DOWN:
                            pieces.getPieces()[idx].setCurrentTile(_tiles[tileIdx + 8]);
                            _tiles[tileIdx + 8].setIsOccupied(true);
                            break;
                        case RIGHT:
                            pieces.getPieces()[idx].setCurrentTile(_tiles[tileIdx + 1]);
                            _tiles[tileIdx + 1].setIsOccupied(true);
                            break;
                        default:
                            break;
                    }
                    _tiles[tileIdx].setIsOccupied(false);
                    ++moveCount;
                }
            }

            if (logic.hasWon())
            {
                font.RenderText("You won!", _width / 2 - 200, _height / 2, 2.0f,
                           glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (logic.hasLost())
            {
                font.RenderText("You lost :(", _width / 2 - 200, _height / 2, 2.0f,
                           glm::vec3(1.0f, 0.0f, 0.0f));
            }

            if (!logic.isGameFinished())
                for (auto pawn : pieces.getPieces())
                    pawn.draw();
        }


        // Swap the screen buffers
        glfwSwapBuffers( _window );
    }
}

void GameWindow::destroy()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
    glDeleteVertexArrays( 1, &BoardVAO );
    glDeleteBuffers( 1, &BoardVBO );

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
}
