#include <SDL2/SDL.h>
//#include "chip8.h"
#include <stdbool.h>

struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

int main(int argc, char* argv[]) {
    
    SDL_Init(SDL_INIT_EVERYTHING);
    struct Game game;

    (game.window) = SDL_CreateWindow("CHIP8", 
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        640, 320, SDL_WINDOW_SHOWN);  
    
    (game.renderer) = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;

    while(!quit) {

        SDL_Event e;

        SDL_WaitEvent(&e);

        if(e.type == SDL_QUIT) quit = true;

    } 

    //chip8_initialize();
    //chip8_loadgame(argv[1]);

    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();

}