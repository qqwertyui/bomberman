cmake_minimum_required(VERSION 3.16.3)
project(bomberman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS "-DENABLE_LOGS")
set(BUILD_UNIT_TESTS "TRUE")

add_subdirectory(src)