cmake_minimum_required(VERSION 3.20)
project(magic_generator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-march=native -Ofast -lpthread")

add_executable(magic_generator main.cpp generation_functions.cpp magics.cpp magics.h bit_manip.h)
