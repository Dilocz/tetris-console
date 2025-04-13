#include <stdint.h>
#ifndef GAME_H
#define GAME_H GAME_H

void gameSetup();
void gameReset();
void gameKeyHandler(uint8_t key);
void gameUpdate();
void gameDraw();

#endif