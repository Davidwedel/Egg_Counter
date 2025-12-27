#include <Arduino.h>

int prevPinState;
int pulseCounter = 0;
int eggsPerCycle = 6;
unsigned long totalEggs = 0;
const int beamThreshold = 600;

// pins
const int sensorPin = A5;
const int pulsePin = 2;
const int pulsePin2 = 11;
const int ledPin = 12;
const int blinkyPin = 13;

// Timing
unsigned long prevCheck = 0;
unsigned long prevBlink = 0;
unsigned long prevDiagnostic = 0;

// intervals
const unsigned long checkInterval = 50;
const unsigned long blinkInterval = 50;
const unsigned long diagnosticInterval = 5000;

int pulseState = LOW;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(pulsePin, OUTPUT);
  pinMode(pulsePin2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(blinkyPin, OUTPUT);
  Serial.begin(115200);
  prevPinState = analogRead(sensorPin);
}

void loop() {
  unsigned long now = millis();

  // check input every 10 ms
  if (now - prevCheck >= checkInterval) {
    prevCheck = now;
    int pinState = analogRead(sensorPin);
    // Serial.println(pinState);

    // difference greater than threshold
    int diff = prevPinState - pinState;
    // Serial.print("diff");
    // Serial.println(diff);
    if (diff > beamThreshold) {
      pulseCounter += eggsPerCycle;
      totalEggs += eggsPerCycle;
      // Serial.println(eggsPerCycle);
      // Serial.println(pulseCounter);
    }
    prevPinState = pinState;

    // shows status of beam. High beam broken, Low beam shining through
    if (abs(diff) > beamThreshold)
      digitalWrite(ledPin, !digitalRead(ledPin));
  }

  // Handle pulsing
  if (pulseCounter > 0 && (now - prevBlink >= blinkInterval)) {
    prevBlink = now;

    // Toggle counter on rotem
    pulseState = !pulseState;

    digitalWrite(pulsePin, pulseState);
    digitalWrite(pulsePin2, pulseState);

    if (pulseState == LOW) {
      pulseCounter--;
      // Serial.println(pulseCounter);
    }
  }

  // Diagnostic output every 5 seconds
  if (now - prevDiagnostic >= diagnosticInterval) {
    prevDiagnostic = now;

    // Toggle blinkyPin
    digitalWrite(blinkyPin, HIGH);

    if (Serial) { // Only print if Serial is connected
      Serial.print("Total Eggs: ");
      Serial.print(totalEggs);
      Serial.print(" | pulseCounter: ");
      Serial.print(pulseCounter);
      Serial.print(" | uptime: ");
      unsigned long uptimeSeconds = now / 1000;
      unsigned long minutes = uptimeSeconds / 60;
      unsigned long seconds = uptimeSeconds % 60;
      Serial.print(minutes);
      Serial.print("m ");
      Serial.print(seconds);
      Serial.println("s");
    }

    delay(100);
    digitalWrite(blinkyPin, LOW);
  }

  delay(1);
}
