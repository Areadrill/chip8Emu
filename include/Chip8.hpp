#include <cstdint>
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>

class Chip8{
    private:
        std::vector<uint16_t> rom;

        uint8_t registers[16];
        uint16_t I;
        uint16_t pc;

        uint16_t stack[24];

        bool screen[32][64];
        bool keyboard[16];

        uint16_t toShort(uint8_t, uint8_t);
        std::vector<uint16_t> loadROM(char*);

    public:
        Chip8(char*);
};
