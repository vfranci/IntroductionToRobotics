// Define connections to the shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

// Buttons
const int pauseButtonPin = 8;
const int lapButtonPin = 3;
const int resetButtonPin = 2;

// States to consider
bool startPause = false;
bool normalState = true;
bool lapViewState = false;

int pauseButtonState = HIGH;   // Current state of the button
int lastPauseButtonState = HIGH;  // Previous state of the button
unsigned long lastDebounceTime = 0;  // Last time the button state changed
unsigned long debounceDelay = 50;

int lapButtonState = HIGH;  
int lastLapButtonState = HIGH;  
unsigned long lastDebounceTimeLap = 0;  
unsigned long debounceDelayLap = 50;

int resetButtonState = HIGH;  
int lastResetButtonState = HIGH;  
unsigned long lastDebounceTimeReset = 0;  
unsigned long debounceDelayReset = 50;

// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; // Number of digits in the display

// Define the number of unique encodings (0-9)
const int encodingsNumber = 10;
byte dpEncoding = B00000001;
byte off = B00000000;
byte byteEncodings[encodingsNumber] = {
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};
// Variables for laps management
const int maxLaps = 4;
float lapsSaved[maxLaps];
int currentLap;
int lapViewIndex = 0;

// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 100; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed

// Consts 
const int initialStateNum = 0;
const int maxLim = 10000;
const int zero = 0;
const float floatTen = 10.0;
const int ten = 10;

void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(pauseButtonPin, INPUT_PULLUP);
  pinMode(lapButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);

  // Initialize digit control pins 
  for (int i = zero; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], HIGH);
  }
  Serial.begin(9600);
}

void loop() {
  int readingPause = digitalRead(pauseButtonPin);

  if (readingPause != lastPauseButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingPause != pauseButtonState) {
      pauseButtonState = readingPause;

      if (pauseButtonState == LOW) {
        Serial.println("Pause Button Pressed");

        // Toggle startPause state
        startPause = !startPause;
        Serial.print("Start/Pause State: ");
        Serial.println(startPause);
      }
    }
  }

  lastPauseButtonState = readingPause;

  if (startPause) {
    // If in start state, continue updating the stopwatch
    updateStopwatch();
  } else {
    if(!lapViewState){
    // Pause state
    writeNumber(number);
  }
  }

  int readingLap = digitalRead(lapButtonPin);

  if (readingLap != lastLapButtonState) {
    lastDebounceTimeLap = millis();
  }

  if ((millis() - lastDebounceTimeLap) > debounceDelayLap) {
    if (readingLap != lapButtonState) {
      lapButtonState = readingLap;

      if (lapButtonState == LOW) {
        if (normalState) {
          // Save lap time only in normal state
          Serial.println("Lap Button Pressed in Normal State");
          lapsSaved[currentLap] = (number % ten) / floatTen + number / floatTen;
          Serial.print("Lap no. ");
          Serial.print(currentLap);
          Serial.print(" = ");
          Serial.println(lapsSaved[currentLap]);
          currentLap++;

          if (currentLap >= maxLaps) {
            currentLap = zero;
          }
        }
        if (lapViewState) {
          Serial.println("Lap Button Pressed in Lap View State");
          Serial.print("Lap no. ");
          Serial.print(lapViewIndex);
          Serial.print(" = ");
          Serial.println(lapsSaved[lapViewIndex]);
          lapViewIndex++;
          if (lapViewIndex > maxLaps - 1) {
            lapViewIndex = zero;
          }
        }
      }
    }
  }
  if (!startPause) {
    if (lapViewState) {
      // Displaying the recorded laps
      int var = int((lapsSaved[lapViewIndex]) * ten);
      writeNumber(var);
    }
  }

  lastLapButtonState = readingLap;

  int readingReset = digitalRead(resetButtonPin);

  if (readingReset != lastResetButtonState) {
    lastDebounceTimeReset = millis();
  }

  if ((millis() - lastDebounceTimeReset) > debounceDelayReset) {
    if (readingReset != resetButtonState) {
      resetButtonState = readingReset;

      if (resetButtonState == LOW) {
        Serial.println("Reset Button Pressed");

        if (!startPause) {
          number = initialStateNum;
          lapViewState = true;
          normalState = false;
          Serial.print("Lap View: ");
          Serial.println(lapViewState);
        } else {
          lapViewState = false;
          normalState = true;
        }
      }
    }
  }

  lastResetButtonState = readingReset;
}

void updateStopwatch() {
  if (millis() - lastIncrement > delayCount) {
    number++;
    number %= maxLim; // Wrap around after 9999
    lastIncrement = millis();
  }

  // Display the incremented number on the 7-segment display using multiplexing
  writeNumber(number);
}

void writeReg(int digit) {
  // Prepare to shift data by setting the latch pin low
  digitalWrite(latchPin, LOW);
  // Shift out the byte representing the current digit to the shift register
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  // Latch the data onto the output pins by setting the latch pin high
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = zero; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentNumber = number;
  for (int displayDigit = displayCount - 1; displayDigit >= zero; displayDigit--) {
    int digit = currentNumber % ten;
    activateDisplay(displayDigit);

    if (displayDigit == 2) {
      writeReg(byteEncodings[digit] | dpEncoding);
    } else {
      writeReg(byteEncodings[digit]);
    }

    writeReg(off);
    currentNumber /= ten;
  }
}
