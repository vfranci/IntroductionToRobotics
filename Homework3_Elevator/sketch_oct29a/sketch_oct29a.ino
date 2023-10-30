
const int caller1stFloor = 2;  
const int index1stFloor = 11;    

const int caller2ndFloor = 3;  
const int index2ndFloor = 12;   

const int caller3rdFloor = 4;  
const int index3rdFloor = 13;  

//each time the program starts, the elevator is on the first floor
int val1stFloor = HIGH;        
int callerVal1stFloor;            
int lastcallerVal1stFloor = LOW;  

int val2ndFloor = LOW;        
int callerVal2ndFloor;            
int lastcallerVal2ndFloor = LOW;  

int val3rdFloor = LOW;        
int callerVal3rdFloor;            
int lastcallerVal3rdFloor = LOW;  

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;   

unsigned long lastPressTime = 0;
unsigned long currentTime;

const int indexMovement = 10;  
int valMovement = LOW;

const long intervalBlink = 250;
const long interval2Seconds = 2000;   
const long interval3Seconds = 3000;   
const long interval1Second = 1000;  

const long freq1000 = 1000;
const long freq800 = 800;

const int buzzerPin = 9;


void setup() {
  pinMode(caller1stFloor, INPUT_PULLUP);
  pinMode(index1stFloor, OUTPUT);
  digitalWrite(index1stFloor, val1stFloor);

  pinMode(caller2ndFloor, INPUT_PULLUP);
  pinMode(index2ndFloor, OUTPUT);

  pinMode(caller3rdFloor, INPUT_PULLUP);
  pinMode(index3rdFloor, OUTPUT);

  pinMode(indexMovement, OUTPUT);
  digitalWrite(indexMovement, valMovement);
}


void loop() {

  //------------------- 1st floor button -------------------------

  int reading1stFloor = digitalRead(caller1stFloor);
  if (reading1stFloor != lastcallerVal1stFloor) {
    lastDebounceTime = millis();
  }

  // descending from second to first floor; the val variable indicates the floor the elevator is currently on
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (val2ndFloor == HIGH && reading1stFloor == LOW) {
        // first floor callbutton has been pressed
        delay(interval2Seconds);

        digitalWrite(index2ndFloor, HIGH);
        tone(buzzerPin, freq800);
        delay(interval1Second);
        digitalWrite(index2ndFloor, LOW);
        noTone(buzzerPin);
        
        currentTime = millis();
        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }
        tone(buzzerPin, freq1000, intervalBlink); // elevator has arrived at the destination
        digitalWrite(index1stFloor, HIGH);
        val2ndFloor = LOW;
        val1stFloor = HIGH;
        val3rdFloor = LOW;
      
      }

      // descending from third to first floor; the val variable indicates the floor the elevator is currently on
      if (val3rdFloor == HIGH && reading1stFloor == LOW) {
        // first floor callbutton has been pressed
        // the buzzer is buzzing to indicate the doors are closing
        delay(interval2Seconds);

        digitalWrite(index3rdFloor, HIGH);
        tone(buzzerPin, freq800);
        delay(interval1Second);
        digitalWrite(index3rdFloor, LOW);
        noTone(buzzerPin);
        
        currentTime = millis();
        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }

        // the elevator is at the 2nd floor
        digitalWrite(index2ndFloor, HIGH);
        delay(interval1Second);
        digitalWrite(index2ndFloor, LOW);

        //the elevator is moving from 2nd floor to 1st floor
        currentTime = millis();
        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }
        tone(buzzerPin, freq1000, intervalBlink);
        digitalWrite(index1stFloor, HIGH);
        val2ndFloor = LOW;
        val1stFloor = HIGH;
        val3rdFloor = LOW;
      
      }
  }
  lastcallerVal1stFloor = reading1stFloor;

  //------------  2nd floor button  ----------------------
  

  int reading2ndFloor = digitalRead(caller2ndFloor);

  if (reading2ndFloor != lastcallerVal2ndFloor) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
      if (val1stFloor == HIGH && reading2ndFloor == LOW) {
        delay(interval2Seconds);

        digitalWrite(index1stFloor, HIGH);
        tone(buzzerPin, freq800);
        delay(interval1Second);
        digitalWrite(index1stFloor, LOW);
        noTone(buzzerPin);
        
        currentTime = millis();
        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }
        
        tone(buzzerPin, freq1000, intervalBlink);
        digitalWrite(index2ndFloor, HIGH);
        val1stFloor = LOW;
        val2ndFloor = HIGH;
        val3rdFloor = LOW;
      } 

      if (val3rdFloor == HIGH && reading2ndFloor == LOW) {
        delay(interval2Seconds);

        digitalWrite(index3rdFloor, HIGH);
        tone(buzzerPin, freq800);
        delay(interval1Second);
        digitalWrite(index3rdFloor, LOW);
        noTone(buzzerPin);
        
        currentTime = millis();
        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }
        tone(buzzerPin, freq1000, intervalBlink);
        digitalWrite(index2ndFloor, HIGH);
        val1stFloor = LOW;
        val2ndFloor = HIGH;
        val3rdFloor = LOW;

      
      }
    }
  

  lastcallerVal2ndFloor = reading2ndFloor;


  //----------- 3rd floor button ---------

  int reading3rdFloor = digitalRead(caller3rdFloor);
  if (reading3rdFloor != lastcallerVal3rdFloor) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    //ascending from 2nd to 3rd floor
      if (val2ndFloor == HIGH && reading3rdFloor == LOW) {
        delay(interval2Seconds);

        digitalWrite(index2ndFloor, HIGH);
        tone(buzzerPin, freq800);
        delay(interval1Second);
        digitalWrite(index2ndFloor, LOW);
        noTone(buzzerPin);
        currentTime = millis();

        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }
        tone(buzzerPin, freq1000, intervalBlink);
        digitalWrite(index3rdFloor, HIGH);
        val1stFloor = LOW;
        val3rdFloor = HIGH;
        val2ndFloor = LOW;
      }
      // ascending from 1st to 3rd floor; 
      if (val1stFloor == HIGH && reading3rdFloor == LOW) {
        // 3rd floor callbutton has been pressed
        // the buzzer is buzzing to indicate the doors are closing
        delay(interval2Seconds);

        digitalWrite(index1stFloor, HIGH);
        tone(buzzerPin, freq800);
        delay(interval1Second);
        digitalWrite(index1stFloor, LOW);
        noTone(buzzerPin);
        
        currentTime = millis();
        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }

        // the elevator is at the 2nd floor
        digitalWrite(index2ndFloor, HIGH);
        delay(interval1Second);
        digitalWrite(index2ndFloor, LOW);

        //the elevator is moving from 2nd floor to 3rd floor
        currentTime = millis();
        while (millis() - currentTime < interval3Seconds) {
          digitalWrite(indexMovement, HIGH);
          delay(intervalBlink);
          digitalWrite(indexMovement, LOW);
          delay(intervalBlink);
        }
        tone(buzzerPin, freq1000, intervalBlink);
        digitalWrite(index3rdFloor, HIGH);
        val2ndFloor = LOW;
        val3rdFloor = HIGH;
        val1stFloor = LOW;
      
      }
    }
  
  lastcallerVal3rdFloor = reading3rdFloor;

  
}

