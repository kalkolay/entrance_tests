add_executable(server main.cpp)

set_target_properties(server PROPERTIES CXX_STANDARD 17)

target_link_libraries(server sockets)

if(WIN32)
target_link_libraries(server WS2_32)
endif()