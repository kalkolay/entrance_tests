cmake_minimum_required(VERSION 3.20)
project(SPL_Assignment)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TASKS
    Task1
    Task2
    Task3
    Task4
    Task5
)

foreach(TASK ${TASKS})
    file(GLOB SOURCE
        "src/${TASK}/*.h"
        "src/${TASK}/*.cpp"
    )
    add_executable(${TASK} ${SOURCE})
endforeach(TASK)
