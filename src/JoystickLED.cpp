#include<Arduino.h>

// Adjust these to your actual GPIOs connected to the joystick
const int joyX = 34;  // ADC1 channel 6 (GPIO34) - input only
const int joyY = 35;  // ADC1 channel 7 (GPIO35) - input only

const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 16;
const int ledPin4 = 17;

int xPos = 0;
int yPos = 0;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  Serial.begin(115200);
  Serial.println("Move joystick around to control LEDs");
}

void loop() {
  xPos = analogRead(joyX);
  yPos = analogRead(joyY);

  Serial.print("X: ");
  Serial.print(xPos);
  Serial.print(" | Y: ");
  Serial.println(yPos);

  // Adjusted thresholds for 0–4095 range
  if (xPos < 1000 && yPos > 3000) {
    digitalWrite(ledPin1, HIGH);  // Top-left
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
  } else if (xPos > 3000 && yPos > 3000) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);  // Top-right
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
  } else if (xPos > 3000 && yPos < 1000) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);  // Bottom-right
    digitalWrite(ledPin4, LOW);
  } else if (xPos < 1000 && yPos < 1000) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, HIGH);  // Bottom-left
  } else {
    digitalWrite(ledPin1, HIGH);  // Center: All ON
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
  }

  delay(200);
}
