#ifndef CHIP8
#define CHIP8

void chip8_initialize();
void chip8_loadgame(char *game);
void chip8_emulatecycle();
void chip8_setkeys();
void chip8_tick();
#endif