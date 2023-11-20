#include <EEPROM.h>

// Pins 
const int greenPin = 6;
const int bluePin = 5;
const int redPin = 7;
const int pinLDR = A0;
const int trigPin = 9;
const int echoPin = 10;

// Variables to read data from sensors
int valueLDR;  // the analog reading from the sensor divider
int lastValueLDR = 0;
int minValueLDR = 0; // value for min treshold for LDR
long duration = 0;
int distance = 0;
int lastDistance = 0;
int minValue = 5; // value for min treshold for ultrasonic

int samplingInterval = 5000;
bool autoMode = true;

// Arrays and values to store and display logged data using EEPROM
const int dataSize = 10;
int dataLDR[dataSize];
int dataUltrasonic[dataSize];

int eepromLDRAddress = 0;
int eepromUltrasonicAddress = dataSize * sizeof(float); // using float to leave spaces between int values

const int lightOn = 255;
const int lightOff = 0;
int colourRGB;
int colourRGBAlert;

const float speedOfSound = 0.034;
const int redVal = 1;
const int greenVal = 2;
const int blueVal = 3;

const int userInputOne = 1;
const int userInputTwo = 2;
const int userInputThree = 3;
const int userInputFour = 4;

const int twoMicros = 2;
const int tenMicros = 10;
const int thousandMillis = 1000;
const int two = 2;
const int zero = 0;
const int stopVal = 0;
const int runningVal = 1;
int baudRate = 9600;

bool alert = false;

void setup() {
  Serial.begin(baudRate);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  //Empty previous data saved in EEPROM
  resetData(eepromLDRAddress, dataLDR, dataSize);
  resetData(eepromUltrasonicAddress, dataUltrasonic, dataSize);
  // Display the initial menu interface to the Serial Monitor upon startup
  printMainMenu();
}
void loop() {
  sensorsMeasurements();
  unsigned long currentMillis = millis();
  unsigned long previousMillis = zero;
  if (currentMillis - previousMillis >= samplingInterval) {
    int logIndex = (currentMillis / samplingInterval) % dataSize;
    dataLDR[logIndex] = valueLDR;
    dataUltrasonic[logIndex] = distance;
    writeData(eepromLDRAddress, dataLDR, dataSize);
    writeData(eepromUltrasonicAddress, dataUltrasonic, dataSize);
    previousMillis = currentMillis;
  }
  // Continuously checks for incoming serial data
  if (Serial.available()) {
    int option = Serial.parseInt();
    mainMenu(option); 
  }
}

void sensorsMeasurements(){
  valueLDR = analogRead(pinLDR);
    if(valueLDR < minValueLDR){
      if(autoMode){
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, lightOff);
      } else {
        if(colourRGBAlert == redVal){ // if red was set for alerts in manual mode
          digitalWrite(redPin, lightOn);
          digitalWrite(greenPin, lightOff);
          digitalWrite(bluePin, lightOff);
        }
        if(colourRGBAlert == greenVal){
          digitalWrite(redPin, lightOff);
          digitalWrite(greenPin, lightOn);
          digitalWrite(bluePin, lightOff);
        }
        if(colourRGBAlert == blueVal){
          digitalWrite(redPin, lightOff);
          digitalWrite(greenPin, lightOff);
          digitalWrite(bluePin, lightOn);
        }
      }
      valueLDR = lastValueLDR;
      Serial.println("Minimum value exceeded. Environment is too dark. Logging last value measured.");
    } else {
      if(autoMode){
        digitalWrite(redPin, lightOff);
        digitalWrite(greenPin, lightOn);
        digitalWrite(bluePin, lightOff);
      } else {
        if(colourRGB == redVal){ // if red was set for normal state in manual mode
          digitalWrite(redPin, lightOn);
          digitalWrite(greenPin, lightOff);
          digitalWrite(bluePin, lightOff);
        }
        if(colourRGB == greenVal){
          digitalWrite(redPin, lightOff);
          digitalWrite(greenPin, lightOn);
          digitalWrite(bluePin, lightOff);
        }
        if(colourRGB == blueVal){
          digitalWrite(redPin, lightOff);
          digitalWrite(greenPin, lightOff);
          digitalWrite(bluePin, lightOn);
        }
     }
  }
  lastValueLDR = valueLDR;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(twoMicros);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(tenMicros);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // Sound wave reflects from the obstacle, so to calculate the distance we
  // consider half of the distance traveled.
  distance = duration * speedOfSound / two;
  if(distance < minValue){
    if(autoMode){
      digitalWrite(redPin, lightOn);
      digitalWrite(greenPin, lightOff);
    } else {
      if(colourRGBAlert == redVal){ 
        digitalWrite(redPin, lightOn);
        digitalWrite(greenPin, lightOff);
        digitalWrite(bluePin, lightOff);
      }
      if(colourRGBAlert == greenVal){
        digitalWrite(redPin, lightOff);
        digitalWrite(greenPin, lightOn);
        digitalWrite(bluePin, lightOff);
      }
      if(colourRGBAlert == blueVal){
        digitalWrite(redPin, lightOff);
        digitalWrite(greenPin, lightOff);
        digitalWrite(bluePin, lightOn);
      }
    }
    distance = lastDistance;
    Serial.println("Minimum value exceeded. Object is too close. Logging last distance measured");
  } else {
      if(autoMode){
        digitalWrite(redPin, lightOff);
        digitalWrite(greenPin, lightOn);
        digitalWrite(bluePin, lightOff);
      } else {
        if(colourRGB == redVal){ 
          digitalWrite(redPin, lightOn);
          digitalWrite(greenPin, lightOff);
          digitalWrite(bluePin, lightOff);
        }
        if(colourRGB == greenVal){
          digitalWrite(redPin, lightOff);
          digitalWrite(greenPin, lightOn);
          digitalWrite(bluePin, lightOff);
        }
        if(colourRGB == blueVal){
          digitalWrite(redPin, lightOff);
          digitalWrite(greenPin, lightOff);
          digitalWrite(bluePin, lightOn);
        }
     }
  }
  lastDistance = distance;
}

void writeData(int address, int array[], int size) {
  for (int i = 0; i < size; i++) {
    EEPROM.put(address + i * sizeof(float), array[i]);
  }
}

void displayData(int address, int array[], int size) {
  for (int i = 0; i < size; i++) {
    int val = EEPROM.get(address + i * sizeof(float), array[i]);
    Serial.print("Value no. ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(val);
  }
}

void resetData(int address, int array[], int size) {
  for (int i = 0; i < size; i++) {
    array[i] = 0;
    EEPROM.put(address + i * sizeof(float), array[i]);
  }
}


// Function to display a menu of options to the user
void printMainMenu() {
  Serial.println("\nMenu Interface:");
  Serial.println("\nSelect an option:");
  Serial.println("1. Sensor settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}

void printSensorSettings() {
  Serial.println("\nSensor settings:");
  Serial.println("\nSelect an option:");
  Serial.println("1. Sensors Sampling Interval");
  Serial.println("2. Ultrasonic Alert Threshold");
  Serial.println("3. LDR Alert Threshold");
  Serial.println("4. Back");
}

void printResetLoggerData() {
  Serial.println("\nAre you sure that you want to Reset Logger Data?");
  Serial.println("\nSelect an option:");
  Serial.println("1. Yes");
  Serial.println("2. No");
  Serial.println("3. Back");
}

void printSystemStatus() {
  Serial.println("\nSystem Status:");
  Serial.println("\nSelect an option:");
  Serial.println("1. Current Sensor Readings");
  Serial.println("2. Current Sensor Settings");
  Serial.println("3. Display Logged Data");
  Serial.println("4. Back");
}

void printLedControls() {
  Serial.println("\nRGB LED Control:");
  Serial.println("\nSelect an option:");
  Serial.println("1. Manual Color Control");
  Serial.println("2. LED: Toggle Automatic ON/OFF");
  Serial.println("3. Back");
}


void mainMenu(int option) {
  switch (option) {
    case userInputOne: {
      printSensorSettings();
      sensorSettings();
      break;
    }
    case userInputTwo: { 
      printResetLoggerData();
      resetLoggerData(); 
      break;
    }
    case userInputThree: {
      printSystemStatus();
      systemStatus(); 
      break;
    }
    case userInputFour:{
      printLedControls();
      ledControl(); 
      break;
    }
    default: {
    // Handles any choices outside the defined options
      Serial.println("Invalid option. Please try again.");
    }
  }
}

void sensorSettings(){
  while (!Serial.available()) {}
  int userChoicesensorSettings = Serial.parseInt();
  switch (userChoicesensorSettings) {
    case userInputOne: {
      Serial.println("\nInsert a value between 1 and 10 (seconds) to establish a sampling rate for the sensors.");
      while (!Serial.available()) {}
      int samplingValue = Serial.parseInt();
      samplingInterval = samplingValue * thousandMillis;
      Serial.print("A new sample will be collected every ");
      Serial.print(samplingValue);
      Serial.println(" seconds.");
      break;
    }
    case userInputTwo: {
      Serial.println("\nInsert a value to establish the closest distance the Ultrasonic Sensor can measure.");
      while (!Serial.available()) {}
      int tresholdValue = Serial.parseInt();
      minValue = tresholdValue;
      Serial.print("The closest distance the Ultrasonic Sensor can measure is ");
      Serial.print(minValue);
      Serial.println(" cm.");
      break;
    }
    case userInputThree: {
      Serial.println("\nInsert a value to establish the lowest value the LDR Sensor can measure.");
      while (!Serial.available()) {}
      int tresholdLDRValue = Serial.parseInt();
      minValueLDR = tresholdLDRValue;
      Serial.print("The closest distance the Ultrasonic Sensor can measure is ");
      Serial.println(minValueLDR);
      break;
    }
    case userInputFour: {
      break;
    }
    default: {
    // Handles any choices outside the defined options
      Serial.println("Invalid option. Please try again.");
    }
  }
  printMainMenu();
}

void resetLoggerData() {
  while (!Serial.available()) {}
  int userChoiceReset = Serial.parseInt();
  switch (userChoiceReset) {
    case userInputOne: {
      resetData(eepromLDRAddress, dataLDR, dataSize);
      resetData(eepromUltrasonicAddress, dataUltrasonic, dataSize);
      Serial.println("\nData has been reset.");
      break;
    }
    case userInputTwo: {
      Serial.println("\nNo changes made.");
      break;
    }
    case userInputThree: {
      break;
    }
    default: {
      Serial.println("\nInvalid option. Please try again.");
    }
  }
  printMainMenu();
}

void systemStatus() {
  while (!Serial.available()) {}
  int userChoice3 = Serial.parseInt();
  switch (userChoice3) {
    case userInputOne: {
      int stop = runningVal;
      unsigned long currentMillisPrint;
      unsigned long previousMillisPrint = zero;

      while (stop != stopVal) {
        sensorsMeasurements();
        currentMillisPrint = millis();

        if (currentMillisPrint - previousMillisPrint >= samplingInterval) {
          Serial.print("LDR sensor measurement: ");
          Serial.println(valueLDR);
          Serial.print("Ultrasonic sensor measurement: ");
          Serial.println(distance);
          Serial.println("Enter 0 to exit.");
          previousMillisPrint = currentMillisPrint;
        }

        // Check if the user entered 0 to stop displaying
        if (Serial.available()) {
          stop = Serial.parseInt();
        }
      }
      break;
    }
    case userInputTwo: {
      Serial.print("\nSampling rate (seconds): ");
      Serial.println(samplingInterval / thousandMillis);
      Serial.print("Minimum distance measured (cm): ");
      Serial.println(minValue);
      Serial.print("Minimal value measured: ");
      Serial.println(minValueLDR);
      break;
    }
    case userInputThree: {
      displayData(eepromLDRAddress, dataLDR, dataSize);
      displayData(eepromUltrasonicAddress, dataUltrasonic, dataSize);
      break;
    }
    case userInputFour: {
      break;
    }
    default: {
      Serial.println("\nInvalid option. Please try again.");
    }
  }
  printMainMenu();
}

void ledControl(){
  while (!Serial.available()) {}
  int userChoice4 = Serial.parseInt();
  switch(userChoice4) {
    case userInputOne: {
      Serial.println(F("\nSet RGB colours manually. Select which colour you want the RBG to be when any limit is exceeded: "));
      Serial.println("1. Red");
      Serial.println("2. Green");
      Serial.println("3. Blue");
      selectColourAlerts();
      Serial.println(F("\nSelect which colour you want the RBG to be when the system is functioning within the thresholds set: "));
      Serial.println("1. Red");
      Serial.println("2. Green");
      Serial.println("3. Blue");
      selectColour();
      break;
    }
    case userInputTwo: {
      Serial.println(F("\nToggle Automatic ON/OFF: "));
      Serial.println(F("\nIf the automatic mode is ON, default colours are RED for alerts and GREEN for good system functionality."));
      Serial.println(F("If you want to use your own colour code, toggle automatic mode OFF and choose them by pressing 4, then 1 starting in the MAIN MENU."));
      if(autoMode){
        Serial.println(F("\n Automatic mode is ON. Enter 1 to keep it ON or 2 to turn it OFF."));
        autoModeToggle();
      } else {
        Serial.println(F("\n Automatic mode is OFF. Enter 1 to keep it OFF or 2 to turn it ON."));
        autoModeToggle();
      }
      break;
    }
    case userInputThree: {
      break;
    }
    default: {
      Serial.println("\nInvalid option. Please try again.");
    }
  }
  printMainMenu();
}

void autoModeToggle(){
  while (!Serial.available()) {}
  int autoToggle = Serial.parseInt();
  switch(autoToggle) {
    case userInputOne: {
      break;
    }
    case userInputTwo: {
      autoMode = !autoMode;
      break;
    }
    default: {
      Serial.println("\nInvalid option. Please try again.");
    }
  }
  if(autoMode) {
    Serial.println("\nAutomatic mode is ON.");
  } else {
    Serial.println("\nAutomatic mode is OFF. Entering manual mode.");
  }
}

void selectColour(){
  while (!Serial.available()) {}
  int colour = Serial.parseInt();
  switch(colour) {
    case userInputOne: { //red
      colourRGB = redVal;
      break;
    }
    case userInputTwo: { //green
      colourRGB = greenVal;
      break;
    }
    case userInputThree: { //blue
      colourRGB = blueVal;
      break;
    }
    default: {
      Serial.println("\nInvalid option. Please try again.");
    }
  }
  Serial.print("\nSelected colour: ");
  Serial.println(colourRGB);
}

void selectColourAlerts(){
  while (!Serial.available()) {}
  int colourAlert = Serial.parseInt();
  switch(colourAlert) {
    case userInputOne: { //red
      colourRGBAlert = redVal;
      break;
    }
    case userInputTwo: { //green
      colourRGBAlert = greenVal;
      break;
    }
    case userInputThree: { //blue
      colourRGBAlert = blueVal;
      break;
    }
    default: {
      Serial.println("\nInvalid option. Please try again.");
    }
  }
  Serial.print("\nSelected colour: ");
  Serial.println(colourRGBAlert);
}

