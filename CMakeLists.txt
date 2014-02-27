cmake_minimum_required(VERSION 2.6)
project(mpl-interpreter)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(mpli ${sources})

