cmake_minimum_required(VERSION 3.24)
project(SocketSender)

set(CMAKE_CXX_STANDARD 17)

add_executable(SocketSender main.cpp socket_connector.cpp socket_connector.h)

target_link_libraries(SocketSender ws2_32)