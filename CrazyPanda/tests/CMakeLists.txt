cmake_minimum_required(VERSION 3.20)
project(InventoryTests
        VERSION 1.0
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### GoogleTest
add_subdirectory(${EXTERNAL_INSTALL_LOCATION}/googletest build)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
### !GoogleTest

add_executable(InventoryTests InventoryTests.cpp)

target_link_libraries(InventoryTests
        gtest
        gtest_main
        Inventory
        )
