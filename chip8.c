#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//constants used in the program
enum {
    FONTSET_ADDRESS = 0,
    START_ADDRESS = 0x200,
    FONTSET_SIZE = 80
};

// Core attributes of the emulator

uint16_t opcode;
uint8_t registers[16];
uint8_t memory[4096];
uint16_t index;
uint16_t pc;
uint16_t stack[16];
uint8_t sp;
uint8_t delayTimer;
uint8_t soundTimer;
uint8_t keypad[16];
uint32_t video[64 * 32];
uint16_t opcode;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

int randomnumber() {
    return rand() % 256;
}

void chip8_loadgame(char const* filename) {
    FILE* fgame = fopen(filename, "r");

    if(fgame) {
        fprintf(stderr, "Could not open the file");
        exit(EXIT_FAILURE);
    }

    fread(&memory[200], 1, 0x1000 - 0x200, fgame);

    fclose(fgame);
}

void chip8_init() {

    pc = START_ADDRESS;
    opcode = 0;
    srand(time(NULL));

    for(int i = 0; i < FONTSET_SIZE; i++) {
        memory[FONTSET_ADDRESS + i] = fontset[i];
    }
}
void chip8_execute_intruction() {

    int i;
    uint16_t NNN;
    uint8_t NN;
    uint8_t x, y, N;

    opcode = (int) memory[pc] << 8 | (int) memory[pc];
    x = (opcode >> 8) & 0x000F;
    y = (opcode >> 4) & 0x000F;
    N = opcode & 0x000F;
    NN = opcode & 0x00FF;
    NNN =  opcode & 0x0FFF;
    
    switch(opcode & 0xF000) {
        
    }
}
