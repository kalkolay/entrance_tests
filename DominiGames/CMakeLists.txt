cmake_minimum_required(VERSION 3.17)
project(DominiGames)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

set(SOIL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SOIL2/include")
set(SOIL_LIBRARY "${CMAKE_SOURCE_DIR}/SOIL2/libsoil2.a")
set(FREETYPE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/freetype/2.10.4/include/freetype2")
set(FREETYPE_LIBRARY "${CMAKE_SOURCE_DIR}/freetype/2.10.4/lib/libfreetype.dylib")

set(INCLUDE_DIRS
        ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR} ${SOIL_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR})
set(LIBRARIES
        OpenGL::GL ${GLUT_LIBRARY} glfw GLEW::glew ${SOIL_LIBRARY} ${GLM_LIBRARY} ${FREETYPE_LIBRARY})

include_directories(${INCLUDE_DIRS})

add_executable(DominiGames
        main.cpp
        Shader.cpp
        Shader.h
        Piece.cpp
        Piece.h
        GameWindow.cpp
        GameWindow.h
        Tile.cpp
        Tile.h
        GameLogic.cpp
        GameLogic.h
        AI.cpp
        AI.h
        PieceManager.cpp
        PieceManager.h
        Font.cpp
        Font.h
        Menu.cpp
        Menu.h
        LeeAlgorithm.cpp
        LeeAlgorithm.h)

target_link_libraries(DominiGames ${LIBRARIES})