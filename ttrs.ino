#include <stdint.h>
#include "U8glib.h"

#include "Menu.h"
#include "Game.h"
#include "GameOver.h"

#define SCREEN_HEIGHT 128
#define SCREEN_WIDTH 64

// červenné 8
// modré 9
// zelenné 10
// žluté 11
// bílé 12
// černné 7

#define KEY_UP_PIN 8
#define KEY_DOWN_PIN 10
#define KEY_LEFT_PIN 12
#define KEY_RIGHT_PIN 9
#define KEY_ROTATE_PIN 11
#define KEY_ESC_PIN 7

#define DEBOUNCING_PAUSE 40


U8GLIB_ST7920_128X64_1X u8g(2, 3, 4);


enum Keys : uint8_t {
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ROTATE,
  KEY_ESC,
  KEY_COUNT,
  KEY_NONE
};

enum Stage : uint8_t {
  MENU,
  GAME,
  GAME_OVER
};

struct ButtonState {
  uint64_t startTime;
  uint8_t previousState;
  uint8_t buttonState;
};

uint8_t textHeight;
uint8_t redraw = 1;
uint16_t score = 0;
uint16_t lastScore = 0;
uint16_t bestScore = 0;
Stage currentStage = MENU;

ButtonState buttonsStates[KEY_COUNT];

// Functions

uint8_t buttonDebouncing(uint8_t buttonStateId, uint8_t keyPin) {
  ButtonState* btn = &buttonsStates[buttonStateId];

  if (btn->startTime != 0) {
    if (millis() - btn->startTime >= DEBOUNCING_PAUSE) {
      uint8_t currentState = digitalRead(keyPin);
      if (currentState == btn->previousState) {
        btn->buttonState = currentState;
      }
      btn->startTime = 0;
      return btn->buttonState;
    }
    return -1;
  }
  uint8_t currentState = digitalRead(keyPin);
  if (currentState != btn->buttonState) {
    btn->startTime = millis();
    btn->previousState = currentState;
  }
  return -1;
}

void keyDetection() {
  // static uint8_t lastKey = KEY_NONE;
  uint8_t currentKey = KEY_NONE;

  if (buttonDebouncing(KEY_UP, KEY_UP_PIN) == 0) currentKey = KEY_UP;
  else if (buttonDebouncing(KEY_DOWN, KEY_DOWN_PIN) == 0) currentKey = KEY_DOWN;
  else if (buttonDebouncing(KEY_LEFT, KEY_LEFT_PIN) == 0) currentKey = KEY_LEFT;
  else if (buttonDebouncing(KEY_RIGHT, KEY_RIGHT_PIN) == 0) currentKey = KEY_RIGHT;
  else if (buttonDebouncing(KEY_ROTATE, KEY_ROTATE_PIN) == 0) currentKey = KEY_ROTATE;
  else if (buttonDebouncing(KEY_ESC, KEY_ESC_PIN) == 0) currentKey = KEY_ESC;

  if (currentKey != KEY_NONE) {
    switch (currentStage) {
      case MENU:
        menuKeyHandler(currentKey);
        break;
      case GAME:
        gameKeyHandler(currentKey);
        break;
    }
    redraw = 1;
  }
  // lastKey = currentKey;
}

void draw() {
  if (redraw) {
    u8g.firstPage();
    do {
      switch (currentStage) {
        case MENU:
          menuDraw();
          break;
        case GAME:
          gameDraw();
          break;
        case GAME_OVER:
          gameOverDraw();
          break;
      }
    } while (u8g.nextPage());
    redraw = 0;
  }
}

void update() {
  switch (currentStage) {
    case GAME:
      gameUpdate();
      break;
    case GAME_OVER:
      gameOverUpdate();
      break;
  }
}

void setup() {
  pinMode(KEY_UP_PIN, INPUT_PULLUP);
  pinMode(KEY_DOWN_PIN, INPUT_PULLUP);
  pinMode(KEY_LEFT_PIN, INPUT_PULLUP);
  pinMode(KEY_RIGHT_PIN, INPUT_PULLUP);
  pinMode(KEY_ROTATE_PIN, INPUT_PULLUP);
  pinMode(KEY_ESC_PIN, INPUT_PULLUP);

  randomSeed(analogRead(0));

  u8g.setRot270();

  u8g.setFont(u8g_font_6x13);
  u8g.setFontPosTop();

  textHeight = u8g.getFontAscent() - u8g.getFontDescent() + 3;
  menuSetup();
  gameSetup();
  gameOverSetup();
}

void loop() {
  keyDetection();

  static uint64_t lastTime = 0;
  uint64_t currentTime = millis();
  if (currentTime - lastTime >= 100) {
    update();
    draw();
    lastTime = currentTime;
  }
}
