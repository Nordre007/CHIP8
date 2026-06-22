#ifndef CHIP8
#define CHIP8

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

void chip8_init();
void chip8_loadgame(char const* game);
void chip8_execute_instruction();
void chip8_tick();

extern uint8_t keypad[16];
extern uint32_t video[32][64];
extern bool drawFlag;
#endif