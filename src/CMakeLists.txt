add_library(chip8 OBJECT Chip8.cpp)
add_executable(chip8Emu $<TARGET_OBJECTS:chip8> main.cpp)