cmake_minimum_required(VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(sockets STATIC SocketUDP.cpp SocketTCP.cpp)

add_subdirectory(client)
add_subdirectory(server)