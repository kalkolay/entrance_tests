cmake_minimum_required(VERSION 3.20)
project(Inventory
        VERSION 1.0
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### Configs
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/configs DESTINATION "${CMAKE_BINARY_DIR}")
### !Configs

file(GLOB SRC_HEADER_FILES
        "*.h"
        )
file(GLOB SRC_SOURCE_FILES
        "*.cpp"
        )

add_library(Inventory
        STATIC
        ${SRC_HEADER_FILES}
        ${SRC_SOURCE_FILES}
        )

target_link_libraries(Inventory
        nlohmann_json::nlohmann_json
        ${LUA_STATIC_LIBRARY}
        )
