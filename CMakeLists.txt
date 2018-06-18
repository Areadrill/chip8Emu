cmake_minimum_required(VERSION 3.10)
project(Chip8Emu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)

add_subdirectory(src)
