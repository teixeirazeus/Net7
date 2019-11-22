cmake_minimum_required(VERSION 3.15)
project(neural C)

set(CMAKE_C_STANDARD 99)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

add_executable(neural main.c matrix.h)
target_link_libraries(neural m)