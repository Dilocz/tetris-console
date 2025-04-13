#include "Game.h"

#include <stdint.h>
#include "U8glib.h"

#define COLUMNS 10
#define ROWS 20
#define ROTATION_POINT_INDEX 4
#define COUNT_OF_TETROMINOS 7
#define COUNT_OF_BLOCKS 5
#define BLOCK_SIZE 6
#define TOP_PADDING 8
#define LEFT_PADDING 2
#define X_OFFSET 4

extern uint8_t redraw;
extern uint16_t score;
extern uint16_t lastScore;
extern uint16_t bestScore;
extern Stage currentStage;

struct Block {
  uint8_t x;
  uint8_t y;
};

uint8_t places[ROWS][COLUMNS];
Block tetromino[COUNT_OF_BLOCKS];

// private functions

void gameOver() {
  if (score > bestScore) bestScore = score;
  lastScore = score;

  gameReset();

  currentStage = GAME_OVER;
  redraw = 1;
}

uint8_t isBlockAt(int8_t x, int8_t y) {
  return COLUMNS <= x || x < 0 || ROWS <= y || y < 0 || places[y][x];
}

uint8_t canTetrominoMove(int8_t x, int8_t y) {
  for (uint8_t i = 0; i < COUNT_OF_BLOCKS - 1; i++) {
    Block b = tetromino[i];
    if (isBlockAt(b.x + x, b.y + y)) return 0;
  }
  return 1;
}

void moveTetrominoLeft() {
  if (canTetrominoMove(-1, 0)) {
    for (uint8_t i = 0; i < COUNT_OF_BLOCKS; i++) {
      tetromino[i].x--;
    }
  }
}

void moveTetrominoRight() {
  if (canTetrominoMove(1, 0)) {
    for (uint8_t i = 0; i < COUNT_OF_BLOCKS; i++) {
      tetromino[i].x++;
    }
  }
}

void moveTetrominoDown() {
  if (canTetrominoMove(0, 1)) {
    for (uint8_t i = 0; i < COUNT_OF_BLOCKS; i++) {
      tetromino[i].y++;
    }
  } else newBlock();
}

void tetrominoFall() {
  uint8_t i = 1;
  while (canTetrominoMove(0, i)) i++;
  for (uint8_t j = 0; j < COUNT_OF_BLOCKS; j++) tetromino[j].y += i - 1;
  newBlock();
}

uint8_t canRotateTetromino() {
  Block* rp = &tetromino[ROTATION_POINT_INDEX];
  for (uint8_t i = 0; i < ROTATION_POINT_INDEX; i++) {
    Block* bl = &tetromino[i];
    uint8_t oX = bl->x - rp->x;
    uint8_t oY = -(bl->y - rp->y);

    oY = oX ^ oY;
    oX = oX ^ oY;
    oY = oX ^ oY;

    if (isBlockAt(oX + rp->x, oY + rp->y)) return false;
  }
  return true;
}

void rotateTetromino() {
  if (!canRotateTetromino()) return;
  Block* rp = &tetromino[ROTATION_POINT_INDEX];
  for (uint8_t i = 0; i < ROTATION_POINT_INDEX; i++) {
    Block* bl = &tetromino[i];
    uint8_t oX = bl->x - rp->x;
    uint8_t oY = -(bl->y - rp->y);

    oY = oX ^ oY;
    oX = oX ^ oY;
    oY = oX ^ oY;

    bl->x = oX + rp->x;
    bl->y = oY + rp->y;
  }
}

void clearRow(uint8_t row) {
  for (uint8_t i = row - 1; i > 0; i--) {
    for (uint8_t j = 0; j < COLUMNS; j++) {
      places[i + 1][j] = places[i][j];
    }
  }
  score++;
}

void clearFullRows() {
  for (uint8_t i = 0; i < ROWS; i++) {
    uint8_t k = 0;
    for (uint8_t j = 0; j < COLUMNS; j++) {
      if (places[i][j] == 1) k++;
    }
    if (k == COLUMNS) clearRow(i);
  }
}

void setRandomTetromino() {
  uint8_t id = random(7);
  switch (id) {
    case 0:
      tetromino[0] = { X_OFFSET, 0 };
      tetromino[1] = { X_OFFSET + 1, 0 };
      tetromino[2] = { X_OFFSET, 1 };
      tetromino[3] = { X_OFFSET + 1, 1 };
      tetromino[4] = { X_OFFSET + 1, 1 };
      break;
    case 1:
      tetromino[0] = { X_OFFSET, 0 };
      tetromino[1] = { X_OFFSET, 1 };
      tetromino[2] = { X_OFFSET, 2 };
      tetromino[3] = { X_OFFSET, 3 };
      tetromino[4] = { X_OFFSET, 2 };
      break;
    case 2:
      tetromino[0] = { X_OFFSET + 1, 0 };
      tetromino[1] = { X_OFFSET, 1 };
      tetromino[2] = { X_OFFSET + 1, 1 };
      tetromino[3] = { X_OFFSET + 2, 1 };
      tetromino[4] = { X_OFFSET + 1, 1 };
      break;
    case 3:
      tetromino[0] = { X_OFFSET, 0 };
      tetromino[1] = { X_OFFSET + 1, 0 };
      tetromino[2] = { X_OFFSET + 1, 1 };
      tetromino[3] = { X_OFFSET + 2, 1 };
      tetromino[4] = { X_OFFSET + 1, 1 };
      break;
    case 4:
      tetromino[0] = { X_OFFSET + 1, 0 };
      tetromino[1] = { X_OFFSET + 2, 0 };
      tetromino[2] = { X_OFFSET, 1 };
      tetromino[3] = { X_OFFSET + 1, 1 };
      tetromino[4] = { X_OFFSET + 1, 1 };
      break;
    case 5:
      tetromino[0] = { X_OFFSET, 0 };
      tetromino[1] = { X_OFFSET, 1 };
      tetromino[2] = { X_OFFSET + 1, 1 };
      tetromino[3] = { X_OFFSET + 2, 1 };
      tetromino[4] = { X_OFFSET + 1, 1 };
      break;
    case 6:
      tetromino[0] = { X_OFFSET + 2, 0 };
      tetromino[1] = { X_OFFSET, 1 };
      tetromino[2] = { X_OFFSET + 1, 1 };
      tetromino[3] = { X_OFFSET + 2, 1 };
      tetromino[4] = { X_OFFSET + 1, 1 };
      break;
  }
}

void newBlock() {
  for (uint8_t i = 0; i < COUNT_OF_BLOCKS - 1; i++) {
    Block* bl = &tetromino[i];
    places[bl->y][bl->x] = 1;
  }
  setRandomTetromino();

  if (!canTetrominoMove(0, 0)) {
    gameOver();
    return;
  }

  clearFullRows();
}

// public functions

void gameReset() {
  for (uint8_t i = 0; i < ROWS; i++) {
    for (uint8_t j = 0; j < COLUMNS; j++) {
      places[i][j] = 0;
    }
  }
  score = 0;
  setRandomTetromino();
}

void gameSetup() {
  setRandomTetromino();
}

void gameKeyHandler(uint8_t key) {
  switch (key) {
    case KEY_LEFT:
      moveTetrominoLeft();
      break;
    case KEY_RIGHT:
      moveTetrominoRight();
      break;
    case KEY_DOWN:
      moveTetrominoDown();
      break;
    case KEY_UP:
      tetrominoFall();
      break;
    case KEY_ROTATE:
      rotateTetromino();
      break;
    case KEY_ESC:
      currentStage = MENU;
      break;
  }
}
void gameUpdate() {
  static uint8_t counter = 0;
  counter++;
  if (counter == 10) {
    moveTetrominoDown();
    redraw = 1;
    counter = 0;
  }
}
void gameDraw() {
  u8g.drawStr(0, 0, String(score).c_str());

  for (uint8_t i = 0; i < COUNT_OF_BLOCKS - 1; i++) {
    Block b = tetromino[i];
    u8g.drawBox(b.x * BLOCK_SIZE + LEFT_PADDING, b.y * BLOCK_SIZE + TOP_PADDING, BLOCK_SIZE, BLOCK_SIZE);
  }

  for (uint8_t i = 0; i < ROWS; i++) {
    for (uint8_t j = 0; j < COLUMNS; j++) {
      if (places[i][j]) u8g.drawFrame(j * BLOCK_SIZE + LEFT_PADDING, i * BLOCK_SIZE + TOP_PADDING, BLOCK_SIZE, BLOCK_SIZE);
    }
  }
}