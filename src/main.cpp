#include <iostream>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <vector>

uint16_t toShort(uint8_t msb, uint8_t lsb){
    return msb << 8 | lsb;
}

std::vector<uint16_t> loadROM(char *filename){
    std::ifstream rom(filename, std::ios::binary);
    
    if(!rom){
        std::cerr << "Error: No such file found" << std::endl;
    }

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(rom)),(std::istreambuf_iterator<char>()));
    std::vector<uint16_t> opcodes;

    for(std::vector<uint8_t>::iterator it = bytes.begin() ; it != bytes.end(); ++it){
        opcodes.push_back(toShort(*(1+it), *it));
        ++it;
    }

    return opcodes;                        
}

int main(int argc, char **argv){
    std::vector<uint16_t> rom = loadROM("../../roms/pong.rom");

}