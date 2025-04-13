#include "Menu.h"

#include <stdint.h>
#include "U8glib.h"
#include "Game.h"

enum Options : uint8_t {
  CONTINUE,
  NEW_GAME,
  OPTIONS_SIZE
};

extern uint8_t textHeight;
extern Stage currentStage;

int8_t selectedOption = 0;

const char* optionsStrings[OPTIONS_SIZE]{
  "continue",
  "new game"
};

uint8_t optionsLeftPadding[OPTIONS_SIZE];

// private functions

void changeSelected(int8_t amount) {
  selectedOption += amount;
  if (selectedOption >= OPTIONS_SIZE) selectedOption = 0;
  if (selectedOption < 0) selectedOption = OPTIONS_SIZE - 1;
}

void confirmOption() {
  switch (selectedOption) {
    case CONTINUE:
      currentStage = GAME;
      break;
    case NEW_GAME:
      currentStage = GAME;
      gameReset();
      break;
  }
}

// public functions

void menuSetup() {
  for (uint8_t i = 0; i < OPTIONS_SIZE; i++) {
    optionsLeftPadding[i] = (SCREEN_WIDTH - u8g.getStrWidth(optionsStrings[i])) / 2;
  }
}

void menuKeyHandler(uint8_t key) {
  switch (key) {
    case KEY_UP:
      changeSelected(-1);
      break;
    case KEY_RIGHT:
      changeSelected(1);
      break;
    case KEY_ROTATE:
      confirmOption();
      break;
    case KEY_ESC:
      currentStage = GAME;
      break;
  }
}

void menuUpdate() {
}

void menuDraw() {
  for (uint8_t i = 0; i < OPTIONS_SIZE; i++) {
    uint8_t y = i * textHeight;

    if (i == selectedOption) {
      u8g.drawBox(0, y, SCREEN_WIDTH, textHeight);
      u8g.setDefaultBackgroundColor();
    }
    // else u8g.drawFrame(0, y, SCREEN_WIDTH, textHeight);

    u8g.drawStr(optionsLeftPadding[i], y, optionsStrings[i]);
    u8g.setDefaultForegroundColor();
  }

  u8g.drawStr(0, (OPTIONS_SIZE + 2) * textHeight, "Best score:");
  u8g.drawStr(18, (OPTIONS_SIZE + 3) * textHeight, String(bestScore).c_str());

  u8g.drawStr(0, (OPTIONS_SIZE + 4) * textHeight, "Score:");
  u8g.drawStr(18, (OPTIONS_SIZE + 5) * textHeight, String(score).c_str());
}