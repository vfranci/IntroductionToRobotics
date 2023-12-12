#include <LiquidCrystal.h>
#include <LedControl.h>
#include <string.h>
#include <EEPROM.h>

// Pins
const int xPin = A0;
const int yPin = A1;
const int swPin = 2;
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int rs = 9;
const int en = 8;
const int d4 = 7;
const int d5 = 6;
const int d6 = 5;
const int d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lcdBrightnessIndex = 1;
int lcdBrightnessVal[] = {0, 50, 100, 150, 200, 250};
int lastLCDBrightnessIndex = -1;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
int matrixBrightnessIndex = 1;
bool inMatrixBrightness = false;
int matrixBrightnessVal[] = {0, 2, 4, 6, 8, 10};
int lastMatrixBrightnessIndex = 1;
const int matrixSize = 8;
const int matrixStart = 0;
const int matrixEnd = 7;
byte matrix[matrixSize][matrixSize];
const int addr = 0;

byte clover[] = {
  B00000,
  B01010,
  B11011,
  B00100,
  B11011,
  B01010,
  B00000,
  B00000
};

byte arrowRight[] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B10000
};

byte arrowUp[] = {
  B00000,
  B00000,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
  B00000
};

byte arrowDown[] = {
  B00000,
  B00000,
  B10001,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000
};

// Variables and values for joystick 
int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int minThreshold = 350;
int maxThreshold = 650;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;
byte swState = HIGH;
byte lastSwState = HIGH;
unsigned long lastMoveTime = 0;
unsigned long moveDelay = 200;

const String mainMenuOption[] = {"Start Game", "Settings", "About"};
const String settingsOption[] = {"LCD Brightness", "Marix Brightness", "Back"};
bool inMainMenu = false;
bool inGame = false;
bool inSettings = false;
bool inAbout = false;
bool inLCDBrightness = false;

int currentPosition = 0;
int lcdPosition = -1;
int currentPositionSettings = 0;
int lcdPositionSettings = -1;
unsigned long startMillis;

//About vars
const String aboutText = "Game: BombIt!, by: vfranci, GitHub: https://github.com/vfranci";
unsigned long lastMillisAbout = 0;
int textLength = aboutText.length();
const int lcdWidth = 16;  // Adjust the width of your LCD if necessary
int index = 0;
const int intervalAbout = 350;


void setup() {
  pinMode(xPin, INPUT_PULLUP);
  pinMode(yPin, INPUT_PULLUP);
  pinMode(swPin, INPUT_PULLUP);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(1, clover);
  lcd.createChar(2, arrowRight);
  lcd.createChar(3, arrowUp);
  lcd.createChar(4, arrowDown);
  introMessage();
  startMillis = millis();
  Serial.begin(9600);
  lc.shutdown(addr, false);
  lc.setIntensity(addr, matrixBrightnessVal[matrixBrightnessIndex]);
}

void loop() {
  if (millis() - startMillis >= 2000) {
    inMainMenu = true;;
  }

  if(inMainMenu && !inGame && !inSettings){
    if (currentPosition != lcdPosition) {
      lcdPosition = currentPosition;
      mainMenu();
    }
  }

  if(!inGame){
    moveOnLCD();
  }

  if(!inGame && inAbout){
    about();
  }

  if(inSettings){
    if (currentPositionSettings != lcdPositionSettings) {
      lcdPositionSettings = currentPositionSettings;
      settingsMenu();
    }
  }

  if(inMatrixBrightness){
    inSettings = false;
    if (matrixBrightnessIndex != lastMatrixBrightnessIndex) {
      lastMatrixBrightnessIndex = matrixBrightnessIndex;
      matrixBrightness();
    }
  }
  if(inLCDBrightness){
    inSettings = false;
    if (lcdBrightnessIndex != lastLCDBrightnessIndex) {
      lastLCDBrightnessIndex = lcdBrightnessIndex;
      LCDBrightness();
    }
  }
}

void introMessage(){
  lcd.setCursor(4, 0); // set the cursor to column 1, line 0
  lcd.print("Welcome!");
  lcd.setCursor(2, 1);
  lcd.print("Good luck!");
  lcd.write(1);
  lcd.write(1);
}

void moveOnLCD() {
  int reading = digitalRead(swPin);
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        Serial.println("Menu option selected");
         if (inMainMenu && !inSettings) {
          if (currentPosition == 2) {
            inMainMenu = false;
            inAbout = true;
          } else if (currentPosition == 1) {
            inMainMenu = false;
            inSettings = true;
          }
        } else if (inSettings) {
          if (currentPositionSettings == 1) {
            inSettings = false;
            inMatrixBrightness = true;
          } else if (currentPositionSettings == 0) {
            inSettings = false;
            inLCDBrightness = true;
          }
        }
      }
    }
  }

  lastSwState = reading;

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

    if (xValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (inMainMenu) {
        if (currentPosition + 1 == 3) {
          currentPosition = 2;
        } else {
          currentPosition++;
        }
      }
        if (inSettings) {
           Serial.println("Inside inSettings condition");
           Serial.println(currentPositionSettings);
          if (currentPositionSettings + 1 == 3) {
            currentPositionSettings = 2;
          } else {
            currentPositionSettings++;
            Serial.println(currentPositionSettings);
            Serial.println(lcdPositionSettings);
          }
        }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved down");
    }
  }

  if (xValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (inMainMenu) {
        if (currentPosition - 1 < 0) {
          currentPosition = 0;
        } else {
          currentPosition--;
        } 
      }
        if (inSettings) {
          if (currentPositionSettings - 1 < 0) {
            currentPositionSettings = 0;
          } else {
            currentPositionSettings--;
          }
        }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved up");
    }
  }

  if (yValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (inMatrixBrightness) {
        if (matrixBrightnessIndex - 1 < 0)
          matrixBrightnessIndex = 0;
      } else {
        matrixBrightnessIndex--;
      }
      if (inLCDBrightness) {
        if (lcdBrightnessIndex + 1 > 5)
          lcdBrightnessIndex = 5;
      } else {
        lcdBrightnessIndex++;
      }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved left");
    }
  }

  if (yValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (inMatrixBrightness) {
        if (matrixBrightnessIndex + 1 > 5)
          matrixBrightnessIndex = 5;
      } else {
        matrixBrightnessIndex++;
      }
      if (inLCDBrightness) {
        if (lcdBrightnessIndex + 1 > 5)
          lcdBrightnessIndex = 5;
      } else {
        lcdBrightnessIndex++;
      }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println("Moved right");
    }
  }

  if (xValue >= minThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
}


void mainMenu(){
  Serial.println(lcdPosition);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU: scroll ");
  lcd.write(3);
  lcd.write(4);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.print(mainMenuOption[lcdPosition]);
}

void about() {
  lcd.setCursor(0, 1);
  lcd.print("Back to menu: ");
  lcd.write(3);
  lcd.setCursor(0, 0);
  lcd.print(aboutText.substring(index, min(index + lcdWidth, textLength)));

  unsigned long currentMillisAbout = millis();

  // Check if enough time has passed for the next scroll
  if (currentMillisAbout - lastMillisAbout >= intervalAbout) {
    lastMillisAbout = currentMillisAbout;  // Save the last time we shifted

    // Shift characters to the left
    if (index < textLength - lcdWidth) {
      index++;
    } else {
      index = 0; // Reset index for continuous scrolling
    }
  }
    // Check if back to menu
    if (currentPosition != 2) {
      inAbout = false;  // Stop scrolling when navigating back to the menu
      inMainMenu = true;
    }
}

void settingsMenu(){
  Serial.println("in settings");
  Serial.println(lcdPositionSettings);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SETTINGS: ");
  lcd.write(3);
  lcd.write(4);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.print(settingsOption[currentPositionSettings]);
}

void matrixBrightness(){
  Serial.println("in Matrix Brightness");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Back: ");
  lcd.write(3);
  lcd.setCursor(0, 1);
  lcd.print("Brightness: ");
  lcd.print(matrixBrightnessVal[matrixBrightnessIndex]);
  if (currentPositionSettings != 0) {
    Serial.println("Going back");
    inMatrixBrightness = false;  // Stop scrolling when navigating back to the menu
    inSettings = true;
    inMainMenu = false;
    lcdPositionSettings = -1;  // Reset the LCD position for the settings menu
    currentPositionSettings = 0;
    
  }
 lc.setIntensity(addr, matrixBrightnessVal[matrixBrightnessIndex]);
}

void LCDBrightness(){
  Serial.println("in LCD Brightness");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Back: ");
  lcd.write(3);
  lcd.setCursor(0, 1);
  lcd.print("Brightness: ");
  lcd.print(lcdBrightnessVal[lcdBrightnessIndex]);
  if (currentPositionSettings != 0) {
    Serial.println("Going back");
    inLCDBrightness = false;  // Stop scrolling when navigating back to the menu
    inSettings = true;
    inMainMenu = false;
    lcdPositionSettings = -1;  // Reset the LCD position for the settings menu
    currentPositionSettings = 0;
    Serial.println("Settings variables after return:");
    Serial.println("inLCDBrightness: " + String(inLCDBrightness));
    Serial.println("inSettings: " + String(inSettings));
    Serial.println("inMainMenu: " + String(inMainMenu));
    Serial.println("currentPositionSettings: " + String(currentPositionSettings));
  }
}



