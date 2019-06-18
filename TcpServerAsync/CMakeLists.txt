cmake_minimum_required(VERSION 3.8)
project(TcpServerAsync)

set(CMAKE_CXX_STANDARD 11)

add_executable(TcpServerAsync main.cpp TcpServerAsync.cpp TcpServerAsync.h)

target_link_libraries(TcpServerAsync pthread)