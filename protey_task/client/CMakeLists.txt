add_executable(client main.cpp)

set_target_properties(client PROPERTIES CXX_STANDARD 17)

target_link_libraries(client sockets)

if(WIN32)
target_link_libraries(client WS2_32)
endif()