#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

Adafruit_SSD1306 displayInstance(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1);

#define GRID_WIDTH 64
#define GRID_HEIGHT 128

Adafruit_SSD1306 gridDisplay(128, 64, &Wire, -1);

const char shapes_S_left[2][2][4] = { { {0, 0, 1, 1}, {0, 1, 1, 2} }, 
                                      { {0, 1, 1, 2}, {1, 1, 0, 0} } };

const char shapes_S_right[2][2][4] = { { {1, 1, 0, 0}, {0, 1, 1, 2} },
                                       { {0, 1, 1, 2}, {0, 0, 1, 1} } };

const char shapes_L_left[4][2][4] = { { {0, 0, 0, 1}, {0, 1, 2, 2} },
                                      { {0, 1, 2, 2}, {1, 1, 1, 0} }, 
                                      { {0, 1, 1, 1}, {0, 0, 1, 2} },
                                      { {0, 0, 1, 2}, {1, 0, 0, 0} } };

const char shapes_Square[1][2][4] = { { {0, 1, 0, 1}, {0, 0, 1, 1} } };

const char shapes_T[4][2][4] = { { {0, 0, 1, 0}, {0, 1, 1, 2} },
                                 { {0, 1, 1, 2}, {1, 0, 1, 1} },
                                 { {1, 0, 1, 1}, {0, 1, 1, 2} },
                                 { {0, 1, 1, 2}, {0, 0, 1, 0} } };

const char shapes_L_right[2][2][4] = { { {0, 1, 2, 3}, {0, 0, 0, 0} },
                                       { {0, 0, 0, 0}, {0, 1, 2, 3} } };

const short OFFSET_TOP = 19;
const short OFFSET_LEFT = 3;
const short BLOCK_SIZE = 5;
const short TOTAL_TYPES = 6;

word currentPieceType, nextPieceType, currentRotation;
short piecePositionX, piecePositionY;
short currentPiece[2][4];
int pieceInterval = 20, currentScore;
long gameTimer, pieceDelay;

boolean gameGrid[10][18];
boolean button1, button2, button3;

int moveLeftPin = 19;
int moveRightPin = 18;
int rotatePin = 32;
int speedPin = 33;
int startPin = 5;

int restartFlag = 0;
int highScore = 0;

void checkFullLines() {
  boolean isFull;
  for (short y = 17; y >= 0; y--) {
    isFull = true;
    for (short x = 0; x < 10; x++) {
      isFull = isFull && gameGrid[x][y];
    }
    if (isFull) {
      clearLine(y);
      y++;
    }
  }
}

void clearLine(short line) {
  for (short y = line; y >= 0; y--) {
    for (short x = 0; x < 10; x++) {
      gameGrid[x][y] = gameGrid[x][y - 1];
    }
  }
  for (short x = 0; x < 10; x++) {
    gameGrid[x][0] = 0;
  }
  gridDisplay.invertDisplay(true);
  delay(50);
  gridDisplay.invertDisplay(false);
  currentScore += 10;
}

bool isGameOver() {
  boolean gameOverFlag;
  int filledLines = 0;
  for (short y = 0; y < 19; y++) {
    gameOverFlag = false;
    for (short x = 0; x < 10; x++) {
      gameOverFlag = gameOverFlag || gameGrid[x][y];
    }
    if (gameOverFlag) {
      filledLines++;
    }
  }
  if (filledLines > 18) return true;
  return false;
}

void displayGameOverScreen() {
  gridDisplay.clearDisplay();
  drawLayout();
  displayText("GAMEOVER", 9, 9, 40);
  displayText("Highscore", 10, 5, 50);
  char highscoreText[6];
  itoa(highScore, highscoreText, 10);
  displayText(highscoreText, getNumberLength(highScore), 15, 60);
  displayText("Score", 6, 18, 70);
  char scoreText[6];
  itoa(currentScore, scoreText, 10);
  displayText(scoreText, getNumberLength(currentScore), 15, 80);
  gridDisplay.display();
}

void refreshDisplay() {
  gridDisplay.clearDisplay();
  drawLayout();
  drawGrid();
  drawPiece(currentPieceType, 0, piecePositionX, piecePositionY);
  gridDisplay.display();
}

void drawGrid() {
  for (short x = 0; x < 10; x++)
    for (short y = 0; y < 18; y++)
      if (gameGrid[x][y])
        gridDisplay.fillRect(OFFSET_LEFT + (BLOCK_SIZE + 1) * x, OFFSET_TOP + (BLOCK_SIZE + 1) * y, BLOCK_SIZE, BLOCK_SIZE, WHITE);
}

boolean checkNextHorizontalCollision(short piece[2][4], int offset) {
  for (short i = 0; i < 4; i++) {
    short newX = piecePositionX + piece[0][i] + offset;
    if (newX > 9 || newX < 0 || gameGrid[newX][piecePositionY + piece[1][i]])
      return true;
  }
  return false;
}

boolean checkNextVerticalCollision() {
  for (short i = 0; i < 4; i++) {
    short y = piecePositionY + piece[1][i] + 1;
    short x = piecePositionX + piece[0][i];
    if (y > 17 || gameGrid[x][y])
      return true;
  }
  return false;
}

void generateNewPiece() {
  randomSeed(esp_random() + millis());
  currentPieceType = nextPieceType;
  nextPieceType = random(TOTAL_TYPES);
  piecePositionX = (currentPieceType != 5) ? 4 : 3;
  piecePositionY = 0;
  currentRotation = 0;
  copyPieceToCurrent(piece, currentPieceType, currentRotation);
}

void drawPiece(short type, short rotation, short x, short y) {
  for (short i = 0; i < 4; i++)
    gridDisplay.fillRect(OFFSET_LEFT + (BLOCK_SIZE + 1) * (x + piece[0][i]), OFFSET_TOP + (BLOCK_SIZE + 1) * (y + piece[1][i]), BLOCK_SIZE, BLOCK_SIZE, WHITE);
}

void drawNextPiecePreview() {
  short tempPiece[2][4];
  copyPieceToCurrent(tempPiece, nextPieceType, 0);
  for (short i = 0; i < 4; i++)
    gridDisplay.fillRect(50 + 3 * tempPiece[0][i], 4 + 3 * tempPiece[1][i], 2, 2, WHITE);
}

void copyPieceToCurrent(short piece[2][4], short type, short rotation) {
  switch (type) {
    case 0:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = shapes_L_left[rotation][0][i];
        piece[1][i] = shapes_L_left[rotation][1][i];
      }
      break;
    case 1:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = shapes_S_left[rotation][0][i];
        piece[1][i] = shapes_S_left[rotation][1][i];
      }
      break;
    case 2:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = shapes_S_right[rotation][0][i];
        piece[1][i] = shapes_S_right[rotation][1][i];
      }
      break;
    case 3:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = shapes_Square[0][0][i];
        piece[1][i] = shapes_Square[0][1][i];
      }
      break;
    case 4:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = shapes_T[rotation][0][i];
        piece[1][i] = shapes_T[rotation][1][i];
      }
      break;
    case 5:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = shapes_L_right[rotation][0][i];
        piece[1][i] = shapes_L_right[rotation][1][i];
      }
      break;
  }
}

short getMaxRotationForPiece(short type) {
  if (type == 1 || type == 2 || type == 5)
    return 2;
  else if (type == 0 || type == 4)
    return 4;
  else if (type == 3)
    return 1;
  else
    return 0;
}

boolean canRotatePiece(short rotation) {
  short piece[2][4];
  copyPieceToCurrent(piece, currentPieceType, rotation);
  return !checkNextHorizontalCollision(piece, 0);
}

void drawLayout() {
  gridDisplay.drawLine(0, 15, GRID_WIDTH, 15, WHITE);
  gridDisplay.drawRect(0, 0, GRID_WIDTH, GRID_HEIGHT, WHITE);
  drawNextPiecePreview();
  char scoreText[6];
  itoa(currentScore, scoreText, 10);
  displayText(scoreText, getNumberLength(currentScore), 7, 4);
}

short getNumberLength(int n) {
  short digitCount = 1;
  while (n >= 10) {
    n /= 10;
    digitCount++;
  }
  return digitCount;
}

void displayText(char text[], short length, int x, int y) {
  gridDisplay.setTextSize(1);
  gridDisplay.setTextColor(WHITE);
  gridDisplay.setCursor(x, y);
  gridDisplay.cp437(true);
  for (short i = 0; i < length; i++)
    gridDisplay.write(text[i]);
}

void setup() {
  pinMode(moveLeftPin, INPUT_PULLUP);
  pinMode(moveRightPin, INPUT_PULLUP);
  pinMode(rotatePin, INPUT_PULLUP);
  pinMode(speedPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  Serial.begin(9600); 
  Serial.println("Tetris started ...");
  
  if (!gridDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); 
  }
  gridDisplay.setRotation(1);
  gridDisplay.clearDisplay();

  gridDisplay.drawRect(0, 0, GRID_WIDTH, GRID_HEIGHT, WHITE);
  displayText("TETRIS", 7, 12, 40);
  displayText("GAME", 5, 15, 50);
  gridDisplay.display();

  delay(2000);
  gridDisplay.clearDisplay();
  randomSeed(analogRead(0));
  nextPieceType = random(TOTAL_TYPES);
  drawLayout();
  gridDisplay.display();
  
  generateNewPiece();
  gameTimer = millis();
}

void loop() {
  if(!digitalRead(startPin) && restartFlag == 1){
    restartFlag = 0;
    for (short y = 0; y < 20; y++) {
    for (short x = 0; x < 10; x++) {
      gameGrid[x][y] = false;
    }}
    currentScore = 0;
    gridDisplay.clearDisplay();
    randomSeed(esp_random() + millis());
    nextPieceType = random(TOTAL_TYPES);
    drawLayout();
    gridDisplay.display();
    
    generateNewPiece();
    gameTimer = millis();
  }
  if (restartFlag == 0) {
    if (millis() - gameTimer > pieceInterval) {
      checkFullLines();
      refreshDisplay();
      if (checkNextVerticalCollision()) {
        for (short i = 0; i < 4; i++)
          gameGrid[piecePositionX + piece[0][i]][piecePositionY + piece[1][i]] = 1;
        generateNewPiece();
      } else {
        piecePositionY++;
      }
      gameTimer = millis();
    }

    if (!digitalRead(moveLeftPin)) {
      if (button1) {
        if (!checkNextHorizontalCollision(piece, -1)) {
          piecePositionX--;
          refreshDisplay();
        }
        button1 = false;
      }
    } else {
      button1 = true;
    }
    if (!digitalRead(moveRightPin)) {
      if (button2) {
        if (!checkNextHorizontalCollision(piece, 1)) {
          piecePositionX++;
          refreshDisplay();
        }
        button2 = false;
      }
    } else {
      button2 = true;
    }

