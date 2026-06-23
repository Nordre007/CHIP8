#include <SDL2/SDL.h>
#include "chip8.h"

#define CLOCK_HZ 30
#define CLOCK_RATE_MS ((int) ((1.0 / CLOCK_HZ) * 1000 + 0.5))

struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

void drawToScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // add this
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (video[y][x]) {
                SDL_Rect rect = { x * 10, y * 10, 10, 10 };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

uint8_t keyPressed(SDL_Event e) {
    switch (e.key.keysym.sym) {
        case SDLK_1: return 0x0;
        case SDLK_2: return 0x1;
        case SDLK_3: return 0x2;
        case SDLK_4: return 0x3;
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0x7;
        case SDLK_a: return 0x8;
        case SDLK_s: return 0x9;
        case SDLK_d: return 0xa;
        case SDLK_f: return 0xb;
        case SDLK_z: return 0xc;
        case SDLK_x: return 0xd;
        case SDLK_c: return 0xe;
        case SDLK_v: return 0xf;
        default: return 0xff;
    }
}

int main(int argc, char* argv[]) {
    
    SDL_Init(SDL_INIT_EVERYTHING);
    struct Game game;

    (game.window) = SDL_CreateWindow("CHIP8", 
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        640, 320, SDL_WINDOW_SHOWN);  
    
    (game.renderer) = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

    chip8_init();
    chip8_loadgame(argv[1]);
    
    bool quit = false;
    uint32_t prev = SDL_GetTicks();

    while(!quit) {

        SDL_Event e;
        

         
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: 
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if(keyPressed(e) != 0xff) {
                        keypad[keyPressed(e)] = 1;
                    }
                    break;
                case SDL_KEYUP:
                    if(keyPressed(e) != 0xff) {
                        keypad[keyPressed(e)] = 0;
                    }
                    break;
            }

        }
            
        uint32_t now = SDL_GetTicks();
        double dt = (now - prev);
        if(dt >= CLOCK_RATE_MS) {

            chip8_execute_instruction();

            chip8_tick();

            if(drawFlag) {
                drawToScreen(game.renderer);
                drawFlag = false;
            }

            prev = now;
        }
    } 


    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();

}