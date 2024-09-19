cmake_minimum_required(VERSION 3.21)
project(
    "main"
    VERSION 1.0
    DESCRIPTION "A simple project to demonstrate basic CMake usage" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable(drawBmp)
target_sources(drawBmp PRIVATE src/main.cpp)

