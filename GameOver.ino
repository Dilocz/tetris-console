#include "GameOver.h"

#include <stdint.h>
#include "U8glib.h"
#include "Game.h"

extern Stage currentStage;
extern uint16_t score;
extern uint16_t bestScore;
extern uint8_t textHeight;


char gameOverText[] = "Game over";
uint8_t gameOverLeftPadding;

void gameOverSetup() {
  gameOverLeftPadding = (SCREEN_WIDTH - u8g.getStrWidth(gameOverText)) / 2;
}

void gameOverUpdate() {
  static uint8_t GOUCounter = 0;
  GOUCounter++;
  if (GOUCounter == 50) {
    currentStage = MENU;
    GOUCounter = 0;
    redraw = 1;
  }
}
void gameOverDraw() {
  u8g.drawStr(gameOverLeftPadding, 0, gameOverText);

  u8g.drawStr(0, 2 * textHeight, "Best score:");
  u8g.drawStr(18, 3 * textHeight, String(bestScore).c_str());
  
  u8g.drawStr(0, 4 * textHeight, "Score:");
  u8g.drawStr(18, 5 * textHeight, String(lastScore).c_str());
  
  
}