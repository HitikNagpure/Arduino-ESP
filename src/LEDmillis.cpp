#include<Arduino.h>

const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;
const int ledPin4 = 5;

unsigned long prevTime1 = 0;
unsigned long prevTime2 = 0;
unsigned long prevTime3 = 0;
unsigned long prevTime4 = 0;

void setup() {
  Serial.begin(115200); // ESP32 usually defaults to 115200 baud

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  Serial.println("Blinking LED1 every 1 second");
  Serial.println("Blinking LED2 every 2 seconds");
  Serial.println("Blinking LED3 every 3 seconds");
  Serial.println("Blinking LED4 every 4 seconds");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevTime1 >= 1000) {
    digitalWrite(ledPin1, !digitalRead(ledPin1));
    prevTime1 = currentMillis;
    Serial.println("LED 1 Toggled");
  }

  if (currentMillis - prevTime2 >= 2000) {
    digitalWrite(ledPin2, !digitalRead(ledPin2));
    prevTime2 = currentMillis;
    Serial.println("LED 2 Toggled");
  }

  if (currentMillis - prevTime3 >= 3000) {
    digitalWrite(ledPin3, !digitalRead(ledPin3));
    prevTime3 = currentMillis;
    Serial.println("LED 3 Toggled");
  }

  if (currentMillis - prevTime4 >= 4000) {
    digitalWrite(ledPin4, !digitalRead(ledPin4));
    prevTime4 = currentMillis;
    Serial.println("LED 4 Toggled");
  }

  delay(10);
}
