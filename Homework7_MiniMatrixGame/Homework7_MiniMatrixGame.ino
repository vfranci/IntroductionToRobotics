#include "LedControl.h"

// Pins
const int xPin = A0;
const int yPin = A1;
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int swPin = 2;

// Play field
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;

const int matrixSize = 8;
const int matrixStart = 0;
const int matrixEnd = 7;
byte matrix[matrixSize][matrixSize];

// Displays for the end of the game
byte sadFace[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

byte happyFace[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

// Variables for bombs
const int maxBombs = 3;  
const int bombsStart = 0;

struct Bomb {
  int row;
  int col;
  unsigned long startTime;
  bool active;
};

Bomb bombs[maxBombs];
unsigned long bombBlinkDuration = 3000;  
unsigned long bombExplosionDuration = 1000;  

// Player positions
int rowCurrentPos;
int colCurrentPos;
int rowLastPos;
int colLastPos;

// Variables for ending the game
bool gameLost = false;
bool collision = false;

// Variables and values for joystick 
int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;
byte swState = HIGH;
byte lastSwState = HIGH;
unsigned long lastMoveTime = 0;
unsigned long moveDelay = 200;

// Values for blinking dot - player
unsigned long blinkStartTime = 0;
unsigned long blinkInterval = 500;  

// Directions for player movement
const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

// Other values
const int ledOn = 1;
const int ledOff = 0;
const int stopVal = 0;
const int runningVal = 1;
const int addr = 0;
const int baud = 9600;
const int initialVal = 0;
const int moveOnePos = 1;
const int randomVal = 2;

void setup() {
  pinMode(xPin, INPUT_PULLUP);
  pinMode(yPin, INPUT_PULLUP);
  pinMode(swPin, INPUT_PULLUP);
  Serial.begin(baud);
  lc.shutdown(addr, false);
  lc.setIntensity(addr, matrixBrightness);
  generateField();
}

void loop() {
  int reading = digitalRead(swPin);
  
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        Serial.println("Button Pressed");
        placeBomb();
      }
    }
  }

  lastSwState = reading;

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  if (xValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(DOWN);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved down");
    }
  }

  if (xValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(UP);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved up");
    }
  }

  if (yValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(LEFT);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved left");
    }
  }

  if (yValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(RIGHT);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved right");
    }
  }

  if (xValue >= minThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

  //Blinking player LED
  if (millis() - blinkStartTime >= blinkInterval) {
    matrix[rowLastPos][colLastPos] = !matrix[rowLastPos][colLastPos];
    updateMatrix();
    blinkStartTime = millis();
  }

  // Checking for active bombs and blinking them
  for (int i = bombsStart; i < maxBombs; i++) {
    if (bombs[i].active) {
      if (millis() - bombs[i].startTime <= bombBlinkDuration) {
        matrix[bombs[i].row][bombs[i].col] = !matrix[bombs[i].row][bombs[i].col];
        updateMatrix();
      } else if (millis() - bombs[i].startTime <= bombBlinkDuration + bombExplosionDuration) { 
        explodeBomb(i);
      } else {
        if (!gameLost) {
          clearBomb(i);
        } else {
          gameOver();
        }
      }
    }
  }

  if (collision) {
    gameOver();
  }
  checkGameWon();
}

void updateMatrix() {
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      lc.setLed(addr, row, col, matrix[row][col]);
    }
  }
}

void clearMatrix() {
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      matrix[row][col] = ledOff;
      lc.setLed(ledOff, row, col, matrix[row][col]);
    }
  }
}

void playerMove(int direction) {
  matrix[rowLastPos][colLastPos] = ledOff;
  updateMatrix();
  switch (direction) {
    case UP:
      upMovement();
      break;
    case DOWN:
      downMovement();
      break;
    case LEFT:
      leftMovement();
      break;
    case RIGHT:
      rightMovement();
      break;
  }
  if (matrix[rowCurrentPos][colCurrentPos] == ledOn) {  // Check if player walks into a wall
    collision = true;
  } else {
    matrix[rowCurrentPos][colCurrentPos] = ledOn;
    updateMatrix();
    rowLastPos = rowCurrentPos;
    colLastPos = colCurrentPos;
    blinkStartTime = millis();
  }
}

void upMovement() {
  if (rowLastPos == matrixStart) {
    rowCurrentPos = matrixEnd;
    colCurrentPos = colLastPos;
  } else {
    rowCurrentPos = rowLastPos - moveOnePos;
    colCurrentPos = colLastPos;
  }
}

void downMovement() {
  if (rowLastPos == matrixEnd) {
    rowCurrentPos = matrixStart;
    colCurrentPos = colLastPos;
  } else {
    rowCurrentPos = rowLastPos + moveOnePos;
    colCurrentPos = colLastPos;
  }
}

void leftMovement() {
  if (colLastPos == matrixStart) {
    rowCurrentPos = rowLastPos;
    colCurrentPos = matrixEnd;
  } else {
    rowCurrentPos = rowLastPos;
    colCurrentPos = colLastPos - moveOnePos;
  }
}

void rightMovement() {
  if (colLastPos == matrixEnd) {
    rowCurrentPos = rowLastPos;
    colCurrentPos = matrixStart;
  } else {
    rowCurrentPos = rowLastPos;
    colCurrentPos = colLastPos + moveOnePos;
  }
}

void placeBomb() {
  for (int i = bombsStart; i < maxBombs; i++) {
    if (!bombs[i].active) {
      bombs[i].row = rowCurrentPos;
      bombs[i].col = colCurrentPos;
      bombs[i].startTime = millis();
      bombs[i].active = true;
      matrix[bombs[i].row][bombs[i].col] = ledOn;
      updateMatrix();
      break; 
    }
  }
}

void explodeBomb(int bombIndex) {
  for (int i = matrixStart; i < matrixSize; i++) {
    matrix[bombs[bombIndex].row][i] = ledOn;  
    matrix[i][bombs[bombIndex].col] = ledOn;  
    if (rowCurrentPos == bombs[bombIndex].row || colCurrentPos == bombs[bombIndex].col) {
      gameLost = true;
    }
  }
  updateMatrix();
}

// Clear the areas caught in the explosion
void clearBomb(int bombIndex) {
  matrix[bombs[bombIndex].row][bombs[bombIndex].col] = ledOff;
  for (int i = matrixStart; i < matrixSize; i++) {
    matrix[bombs[bombIndex].row][i] = ledOff;
    matrix[i][bombs[bombIndex].col] = ledOff;
  }
  updateMatrix();
  bombs[bombIndex].active = false;
}

void gameOver() {
  int stop = runningVal;
  clearMatrix();
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      lc.setLed(addr, row, col, sadFace[row][col]);
    }
  }
  Serial.println();
  Serial.println("Enter 0 to play again.");
  while (stop != stopVal) {
    if (Serial.available()) {
      stop = Serial.parseInt();
    }
  }
  generateField();
}

// Randomly generate play area filled with 1s for walls and 0 for walkable spaces
void generateField() {
  rowCurrentPos = matrixStart;
  colCurrentPos = matrixStart;
  rowLastPos = matrixStart;
  colLastPos = matrixStart;
  gameLost = false;
  collision = false;
  randomSeed(millis());
  for (int row = matrixStart + moveOnePos; row < matrixSize; row++) {
    for (int col = matrixStart + moveOnePos; col < matrixSize; col++) {
      matrix[row][col] = random(randomVal);  
    }
  }
  updateMatrix();
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      Serial.print(matrix[row][col]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

// Count the empty spaces to see if matrix was cleared
void checkGameWon() {
  int counter = initialVal;
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      if (matrix[row][col] == ledOff) {
        counter++;
      }
    }
  }
  if (counter == (matrixSize * matrixSize)) {
    gameWon();
  }
}

void gameWon() {
  int stop = runningVal;
  clearMatrix();
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      lc.setLed(addr, row, col, happyFace[row][col]);
    }
  }
  Serial.println();
  Serial.println("Enter 0 to play again.");
  while (stop != stopVal) {
    // Check if the user entered 0 to stop displaying
    if (Serial.available()) {
      stop = Serial.parseInt();
    }
  }
  generateField();
}
