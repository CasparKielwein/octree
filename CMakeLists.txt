cmake_minimum_required(VERSION 3.13)
project(fso_octree)

set(CMAKE_CXX_STANDARD 14)

add_library(fso_octree library.cpp library.h)

add_executable(unit-test tests.cpp)

target_link_libraries(unit-test fso_octree)
