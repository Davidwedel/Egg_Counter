#include <Arduino.h>

int prevPinState;
int pulseCounter = 0;
int eggsPerCycle = 6;

//pins
const int sensorPin = A0;
const int pulsePin = 13;

// Timing
unsigned long prevCheck = 0;
unsigned long prevBlink;

//intervals
const unsigned long checkInterval = 10;
const unsigned long blinkInterval = 200;

int pulseState = LOW;



void setup() {
  pinMode(sensorPin, INPUT);  
  pinMode(pulsePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
	unsigned long now = millis();

	//check input every 10 ms
	if(now - prevCheck >= checkInterval) {
		prevCheck = now;
		int pinState = analogRead(sensorPin);

		// difference greater than 600
		if (prevPinState - pinState > 600) {
			pulseCounter += eggsPerCycle;
			Serial.println(eggsPerCycle);
			prevPinState = pinState;
		}
	}

	// Handle pulsing 
	if (pulseCounter > 0 && (now - prevBlink >= blinkInterval)) {
		prevBlink = now;

		//Toggle counter on rotem
		pulseState = !pulseState;

		digitalWrite(pulsePin, pulseState);

		if(pulseState == LOW){
			pulseCounter--;
		}
	}

	delay(1);
}
