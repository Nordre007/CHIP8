#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//constants used in the program
enum {
    FONTSET_ADDRESS = 0,
    START_ADDRESS = 0x200,
    FONTSET_SIZE = 80,
    GFXROWS = 255,
    GFXCOLS = 255
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
uint32_t video[64][32];
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

void draw(uint8_t xposition, uint8_t yposition, uint8_t numberOfBytes) {

    registers[0xF] = 0;
    for(int i = 0; i < numberOfBytes; i++) {
        uint8_t sprite = memory[index + i];
        
        for(int bitindex = 0; bitindex < 8; i++) {
            uint8_t spritepixel = (sprite >> bitindex) & 0x1;
            uint32_t* pixel = &video[(xposition + bitindex) % 64][(yposition + (7 - bitindex)) % 32];

            if(spritepixel == 1 && *pixel == 1) registers[0xF] = 1;

            *pixel = *pixel ^ spritepixel;
        }
    }
}

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
        case 0x0000: 
            switch (NN) {
                case 0xE0:
                    memset(video, 0, sizeof(video));
                    pc += 2;
                    break;
                case 0xEE:
                    --sp;
                    pc = stack[sp];
                    break;    
                default:
                    break;
            }
        case 0x1000: 
            pc = NNN;
            break;
        case 0x2000: 
            ++sp;
            stack[sp] = pc;
            pc = NNN;
            break;
        case 0x3000:
            if(registers[x] == NN) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x4000:
            if(registers[x] != NN) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x5000:
            if(registers[x] == registers[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x6000:
            registers[x] = NN;
            pc += 2;
            break;
        case 0x7000:
            registers[x] += NN;
            pc += 2;
            break;
        case 0x8000:
            switch(N) {
                case 0:
                    registers[x] = registers[y];
                    break;
                case 1:
                    registers[x] = registers[x] | registers[y];
                    break;
                case 2:
                    registers[x] = registers[x] & registers[y];
                    break;
                case 3:
                    registers[x] = registers[x] ^ registers[y];
                    break;
                case 4:
                    uint16_t sum = registers[x] + registers[y];

                    registers[0xF] = sum > 255 ? 1 : 0;
                    registers[x] = sum & 0xFF;
                    break;
                case 5:

                    registers[0xF] = registers[x] > registers[y] ? 1 : 0;
                    registers[x] -= registers[y];
                    break;
                case 6: 
                    registers[0xF] = registers[x] & 0x01;
                    registers[x] /= 2;
                    break;
                case 7:
                    registers[0xF] = registers[y] > registers[x] ? 1 : 0;
                    registers[x] = registers[y] - registers[x];
                    break;
                case 0xE: 
                    registers[0xF] = registers[x] & 0x01;
                    registers[x] *= 2;
                    break;
                default:
            }
            pc += 2;
            break;
        case 0x9000:
            if(registers[x] != registers[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0xA000:
            index = NNN;
            pc += 2;
            break;
        case 0xB000:
            index = NNN + registers[0];
            pc += 2;
            break;
        case 0xC000:
            registers[x] = randomnumber() & NN;
            pc += 2;
            break;
        case 0xD000:
            draw(registers[x], registers[y], N);
            pc += 2;
            break;
        case 0xE000:
            switch(NN) {
                case 0x9E:
                    pc += (keypad[registers[x]]) ? 4 : 2;
                    break;
                case 0xA1:
                    pc += (!keypad[registers[x]]) ? 4: 2;
                    break;
                default:
            }
        case 0xF000:
            switch (NN) {
                case 0x07:
                    registers[x] = delayTimer;
                    break;
                case 0x0A:
                    while(1) {
                        for(int i = 0; i < 16; i++) {
                            if(keypad[i]) {
                                registers[x] = i;
                                goto got_key_press;
                            }
                        }
                    }
                    got_key_press:
                    pc += 2;
                    break;
                case 0x15:
                    delayTimer = registers[x];
                    pc += 2;
                    break;
                case 0x18:
                    soundTimer = registers[x];
                    pc += 2;
                    break;
                case 0x1E:
                    index += registers[x];
                    pc += 2;
                    break;
                case 0x29:
                    index = 5 * registers[x];
                    pc += 2;
                    break;
                case 0x33:
                    memory[index] = (registers[x] % 1000) / 100;
                    memory[index +1] = (registers[x] % 100) / 10;
                    memory[index + 2] = (registers[x] % 10);
                    pc += 2;
                    break;
                case 0x55:
                    for(int i = 0; i < x; i++) {
                        memory[index + i] = registers[i];
                    }
                    pc += 2;
                    break;
                case 0x65:
                    for(int i = 0; i < x; i++) {
                        registers[i] = memory[index + i];
                    }
                    pc += 2;
                    break;
                default:
                    break;
            }
        default:        
    }

}
void chip8_tick() {
    if (delayTimer > 0) {
        --delayTimer;
    }
    if (soundTimer > 0) {
        --soundTimer;
        if (soundTimer == 0) {
            printf("BEEP!\n");
        }
    }
}
