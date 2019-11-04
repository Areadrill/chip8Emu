#include <iostream>
#include <chrono>
#include <thread>
#include "main.hpp"
#include <SDL.h>
   	
// SDL_Texture* LoadTexture( const std::string &str )
// {
//     // Load image as SDL_Surface
//     SDL_Surface* surface = SDL_LoadBMP( str.c_str() );
//     if ( surface == nullptr )
//     {
//         std::cout << "Failed to load surface " << str
//             << " error : " << SDL_GetError() << std::endl;
//         return nullptr;
//     }
    
//     // SDL_Surface is just the raw pixels
//     // Convert it to a hardware-optimzed texture so we can render it
//     SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );
    
//     // Don't need the orignal texture, release the memory
//     SDL_FreeSurface( surface );
    
//     return texture;
// }

int main(int argc, char **argv){
    Chip8 interpreter("../../roms/pong.rom");

    // interpreter.step();
    // interpreter.step();
    // interpreter.step();
    // interpreter.step();
    // interpreter.step(); 
    // interpreter.step();
    // interpreter.step(); 
    // interpreter.step();
    
    // interpreter.step();
    // interpreter.step();
    // interpreter.step();
    // interpreter.step();
    // interpreter.step(); 
    // interpreter.step();
    // interpreter.step(); 
    // interpreter.step();

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_Window *win = SDL_CreateWindow("Chip8Emu", 100, 100, 64, 48, SDL_WINDOW_SHOWN);

    if (win == nullptr){
	    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	    SDL_Quit();
	    return 1;
    }

    while(true) {
        interpreter.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(x));

        SDL_Surface *windowSurface = SDL_GetWindowSurface(win);

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

    return 0;

}