cmake_minimum_required(VERSION 3.20)
project(CrazyPanda
        VERSION 1.0
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### Dependencies
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

if(NOT EXISTS ${EXTERNAL_INSTALL_LOCATION}/googletest)
    message("Downloading GoogleTest...")
    execute_process(COMMAND git clone https://github.com/google/googletest.git
            WORKING_DIRECTORY ${EXTERNAL_INSTALL_LOCATION}
            )
    message("GoogleTest successfully downloaded.")
endif()

if(NOT EXISTS ${EXTERNAL_INSTALL_LOCATION}/json)
    message("Downloading nlohmann/json...")
    execute_process(COMMAND git clone https://github.com/nlohmann/json.git
            WORKING_DIRECTORY ${EXTERNAL_INSTALL_LOCATION}
            )
    message("nlohmann/json successfully downloaded.")
endif()
### !Dependencies

set(JSON_Install OFF CACHE INTERNAL "")

add_subdirectory(${EXTERNAL_INSTALL_LOCATION}/json build)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
