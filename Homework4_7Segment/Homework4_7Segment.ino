const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
const int segSize = 8;

const int noOfDigits = 8;
byte state = HIGH;
byte swStateVal = LOW;
int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// starting position is on the decimal point, which is the seventh element of the segments array
int currentPosition = 7;

// array to memorise which segments have been pressed and will be kept on until reset
int segmentsON[segSize] = {
  0, 0, 0, 0, 0, 0, 0, 0
};

const int directions = 4;

// matrix that shows how to get from one segment to the other depending on the movement of the joystick;
// elements of the matrix represent the "distance" between the current segment and the destination segment
int movement[noOfDigits][directions] = {
  // up, down, left, right, current segment
  // n/a, g, f, b
  {0, 6, 5, 1},  // a 0
  //a, g, f, n/a
  {-1, 5, 4, 0}, // b 1
  // g, d, e, dp
  {4, 1, 2, 5},  // c 2
  // g, n/a, e, c
  {3, 0, 1, -1}, // d 3
  // g, d, n/a, c
  {2, -1, 0, -2}, // e 4
  // a, g, n/a, b
  {-5, 1, 0, -4}, // f 5
  // a, d, n/a, n/a
  {-6, -3, 0, 0}, // g 6 
  // n/a, n/a, c, n/a
  {0, 0, -5, 0},  // dp 7
};

const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;
int move = 0;

unsigned long previousMillis = 0;
const long interval = 500; 
bool segmentState = LOW;

byte swState = HIGH;
byte lastSwState = HIGH;
unsigned long swPressTime = 0;
unsigned long longPressDuration = 1000;
unsigned long debounceDelay = 50; 

unsigned long lastMoveTime = 0;
unsigned long moveDelay = 500;

void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentDebounceMillis = millis();

  if (currentDebounceMillis - swPressTime >= debounceDelay) {
    swState = digitalRead(pinSW);
    
    if (swState == LOW && lastSwState == HIGH) {
      swPressTime = millis();
    }

    if (swState == HIGH && lastSwState == LOW) {
      // sw has been released
      unsigned long swReleaseTime = millis() - swPressTime;

      if (swReleaseTime >= longPressDuration) {
        reset();
        Serial.println("Long press detected");
      } else {
        segmentsON[currentPosition] = 1;
        Serial.println("Short press detected");
      }
    }

    lastSwState = swState; 
  }
  
  set();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    segmentState = !segmentState;
    digitalWrite(segments[currentPosition], segmentState);
  }
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  
  if (xValue < minThreshold && joyMoved == false) {  //  down movement
      // Check for a delay before processing another movement
      if (millis() - lastMoveTime >= moveDelay) {
        move = movement[currentPosition][down];
        currentPosition = currentPosition + move;
        joyMoved = true;
        Serial.print("xValue: ");   
        Serial.println(xValue);
        Serial.print("yValue: ");
        Serial.println(yValue);
        lastMoveTime = millis();
      }
  }

  if (xValue > maxThreshold && joyMoved == false) { //  up movement
      if (millis() - lastMoveTime >= moveDelay) {
        move = movement[currentPosition][up];
        currentPosition = currentPosition + move;
        joyMoved = true;
        Serial.print("xValue: ");   
        Serial.println(xValue);
        Serial.print("yValue: ");
        Serial.println(yValue);
        lastMoveTime = millis();
      }
  }

  if (yValue < minThreshold && joyMoved == false) {  //  left movement
      if (millis() - lastMoveTime >= moveDelay) {
        move = movement[currentPosition][left];
        currentPosition = currentPosition + move;
        joyMoved = true;
        Serial.print("xValue: ");   
        Serial.println(xValue);
        Serial.print("yValue: ");
        Serial.println(yValue);
        lastMoveTime = millis();
      }
  }

  if (yValue > maxThreshold && joyMoved == false) { //  right movement
      if (millis() - lastMoveTime >= moveDelay) {
        move = movement[currentPosition][right];
        currentPosition = currentPosition + move;
        joyMoved = true;
        Serial.print("xValue: ");   
        Serial.println(xValue);
        Serial.print("yValue: ");
        Serial.println(yValue);
        lastMoveTime = millis();
      }
  }

  if (xValue >= minThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
      joyMoved = false;
  }
}
// setting the current segment, as well as the ones that have been switched on by pressing the button
void set() {
  for (int i = 0; i < segSize; i++) {
    if (i == currentPosition) {
      digitalWrite(segments[i], segmentState);
    } else {
      digitalWrite(segments[i], segmentsON[i]);
    }
  }
}
// switching off all of the segments and moving back to the start position (on the decimal point)
void reset() {
  for (int i = 0; i < segSize; i++) {
    segmentsON[i] = 0;
    digitalWrite(segments[i], segmentsON[i]);
  }
  currentPosition = 7;
  Serial.println("Long press detected");
}
