#include <cstdint>
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <stack>
#include <ctime>
#include <cstring>
#include <cmath>

class Chip8{
    private:

        const uint16_t pcOpcodes[3] = {0x00EE, 0x1000, 0x2000};

        const uint8_t fontset[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0,
            0x20, 0x60, 0x20, 0x20, 0x70, 
            0xF0, 0x10, 0xF0, 0x80, 0xF0, 
            0xF0, 0x10, 0xF0, 0x10, 0xF0, 
            0x90, 0x90, 0xF0, 0x10, 0x10, 
            0xF0, 0x80, 0xF0, 0x10, 0xF0, 
            0xF0, 0x80, 0xF0, 0x90, 0xF0, 
            0xF0, 0x10, 0x20, 0x40, 0x40, 
            0xF0, 0x90, 0xF0, 0x90, 0xF0, 
            0xF0, 0x90, 0xF0, 0x10, 0xF0, 
            0xF0, 0x90, 0xF0, 0x90, 0x90, 
            0xE0, 0x90, 0xE0, 0x90, 0xE0, 
            0xF0, 0x80, 0x80, 0x80, 0xF0, 
            0xE0, 0x90, 0x90, 0x90, 0xE0, 
            0xF0, 0x80, 0xF0, 0x80, 0xF0, 
            0xF0, 0x80, 0xF0, 0x80, 0x80
        };

        uint8_t memory[0xFFF];

        std::vector<uint8_t> rom;

        uint8_t registers[16];
        uint8_t sound, delay;
        uint16_t I;
        uint16_t pc;

        std::stack<uint16_t> addrStack[24];

        bool **screen;
        bool keyboard[16];

        std::map<uint16_t, void (Chip8::*)(uint16_t)> handlerMap;

        uint16_t toShort(uint8_t, uint8_t);
        std::vector<uint8_t> loadROM(char*);
        bool handleOpcode(uint16_t);
        void clearScreen();
        void ret();
        void goTo(uint16_t);
        void call(uint16_t);
        void skipIf(uint16_t);
        void skipIfReg(uint16_t);
        void skipIfKey(uint16_t);
        void setRegister(uint16_t);
        void setI(uint16_t);
        void addToI(uint16_t);
        void add(uint16_t);
        void handleRegisterOperations(uint16_t);
        void randAnd(uint16_t);
        void drawSprite(uint16_t);
        void handlePeripheralOperations(uint16_t);
        void getDelay(uint16_t);
        void getKeyPress(uint16_t);
        void setDelay(uint16_t);
        void setSound(uint16_t);
        void setSpriteAddr(uint16_t);
        void setBCD(uint16_t);
        void regDump(uint16_t);
        void regLoad(uint16_t);

    public:
        Chip8(char*);
        bool** getScreen();
        void step();
};
