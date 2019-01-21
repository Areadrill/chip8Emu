#include <iostream>
#include "main.hpp"

int main(int argc, char **argv){
    Chip8 interpreter("../../roms/pong.rom");

    interpreter.step();
    interpreter.step();
    interpreter.step();
    interpreter.step();
    interpreter.step(); 
    interpreter.step();
    interpreter.step(); 
    interpreter.step();
    
    interpreter.step();
    interpreter.step();
    interpreter.step();
    interpreter.step();
    interpreter.step(); 
    interpreter.step();
    interpreter.step(); 
    interpreter.step();

    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 64; j++){
            if(interpreter.getScreen()[i][j]){
                std::cout << 1;
            }
            else{
                std::cout << 0;
            }
        }
        std::cout << "\n";
    }

}