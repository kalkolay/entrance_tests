#include "Menu.h"

Menu::Menu()
    : shader("shaders/core.vert", "shaders/core.frag")
{
    _tiles.push_back(Tile({0, 0}, 0.125, 0)); // play
    _tiles.push_back(Tile({-0.5, 0}, 0.125, 1)); // info
    _tiles.push_back(Tile({0.5, 0}, 0.125, 2)); // exit
    _tiles.push_back(Tile({-0.85, 0.85}, 0.125, 3)); // return
}

void Menu::init()
{
    shader.init();

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
            {
                    // Positions          // Colors           // Texture Coords
                    0.125f,  0.125f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
                    0.125f, -0.125f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
                    -0.125f, -0.125f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
                    -0.125f,  0.125f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
            };
    GLuint indices[] =
            {  // Note that we start from 0!
                    0, 1, 3, // First Triangle
                    1, 2, 3  // Second Triangle
            };
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
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
}

void Menu::loadTexture(const char* tPath)
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
}

void Menu::drawItem(MenuItems item)
{
    shader.use();
    glm::mat4 transform = glm::mat4(1);

    switch (item)
    {
        case PLAY_BUTTON:
            transform = glm::translate( transform,
                                        glm::vec3( 0, 0, 0.0f ) );
            break;
        case INFO_BUTTON:
            transform = glm::translate( transform,
                                        glm::vec3( -0.50, 0, 0.0f ) );
            break;
        case EXIT_BUTTON:
            transform = glm::translate( transform,
                                        glm::vec3( 0.5, 0, 0.0f ) );
            break;
        default:    // RETURN_BUTTON
            transform = glm::translate( transform,
                                        glm::vec3( -0.85, 0.85, 0.0f ) );
            break;
    }

    glBindVertexArray( VAO );

    GLint transformLocation = glGetUniformLocation( shader.getProgram(), "transform" );
    glUniformMatrix4fv( transformLocation, 1, GL_FALSE, glm::value_ptr( transform ) );

    glActiveTexture( GL_TEXTURE0 + item );
    glBindTexture( GL_TEXTURE_2D, _textures[item] );
    glUniform1i( glGetUniformLocation( shader.getProgram(), "ourTexture1" ), item );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
}
