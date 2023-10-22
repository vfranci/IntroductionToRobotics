const int inputValRosu = A0;
const int outputRosu =  10;
const int inputValVerde = A1;
const int outputVerde =  9;
const int inputValAlbastru = A2;
const int outputAlbastru =  11;
int valCititaRosu = 0;
int outputRosuVal = 0;
int valCititaVerde = 0;
int outputVerdeVal = 0;
int valCititaAlbastru = 0;
int outputAlbastruVal = 0;

const int fromLow = 0;
const int fromHigh = 1023;
const int toLow = 0;
const int toHigh = 255;

void setup() {
pinMode(inputValRosu, INPUT);
pinMode(outputRosu, OUTPUT);
pinMode(inputValVerde, INPUT);
pinMode(outputVerde, OUTPUT);
pinMode(inputValAlbastru, INPUT);
pinMode(outputAlbastru, OUTPUT);
}
void loop() {
valCititaRosu = analogRead(inputValRosu);
outputRosuVal = map(valCititaRosu, fromLow, fromHigh, toLow, toHigh);
analogWrite(outputRosu, outputRosuVal);
valCititaVerde = analogRead(inputValVerde);
outputVerdeVal = map(valCititaVerde, fromLow, fromHigh, toLow, toHigh);
analogWrite(outputVerde, outputVerdeVal);
valCititaAlbastru = analogRead(inputValAlbastru);
outputAlbastruVal = map(valCititaAlbastru, fromLow, fromHigh, toLow, toHigh);
analogWrite(outputAlbastru, outputAlbastruVal);
}