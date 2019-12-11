cmake_minimum_required(VERSION 3.1)
project(biocad_test_task)

set(CMAKE_CXX_STANDARD 17)

add_executable(biocad_test_task main.cpp)
target_link_libraries(biocad_test_task OpenCL)
