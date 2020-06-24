cmake_minimum_required(VERSION 3.16)
project(battlesnake_testharness)

set(CMAKE_CXX_STANDARD 20)

add_library(battlesnake_testharness snake.cpp snake.hpp)