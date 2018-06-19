#include <cstdint>
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <map>
#include <stack>
#include <ctime>

class Chip8{
    private:
        std::vector<uint16_t> rom;

        uint8_t registers[16];
        uint8_t sound, delay;
        uint16_t I;
        uint16_t pc;

        std::stack<uint16_t> addrStack[24];

        bool screen[32][64];
        bool keyboard[16];

        std::map<uint16_t, void (*)(uint16_t)> handlerMap;

        uint16_t toShort(uint8_t, uint8_t);
        std::vector<uint16_t> loadROM(char*);
        void handleOpcode(uint16_t);
        void clearScreen();
        void ret();
        void goTo(uint16_t);
        void call(uint16_t);
        void skipIf(uint16_t);
        void skipIfReg(uint16_t);
        void setRegister(uint16_t);
        void setI(uint16_t);
        void add(uint16_t);
        void handleRegisterOperations(uint16_t);


    public:
        Chip8(char*);
};
