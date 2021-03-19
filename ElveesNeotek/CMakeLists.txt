cmake_minimum_required(VERSION 3.16)
project(task)

#set(Boost_INCLUDE_DIR /opt/homebrew/Cellar/boost/1.75.0_2/include/)
#set(Boost_LIBRARY_DIR /opt/homebrew/Cellar/boost/1.75.0_2/lib/)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost!")
endif()

set(SOURCES
    main.cpp
    executor.cpp
    parser.cpp
)

set(HEADERS
    executor.h
    parser.h
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
