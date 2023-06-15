cmake_minimum_required(VERSION 3.25)
project(Port_Manager C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wvla -Wall -Wextra -Wpedantic -Wdeclaration-after-statement")

add_executable(Port_Manager main.c)
