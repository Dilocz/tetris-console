#include <stdint.h>
#ifndef MENU_H
#define MENU_H MENU_H

void menuSetup();
void menuKeyHandler(uint8_t key);
void menuUpdate();
void menuDraw();

#endif