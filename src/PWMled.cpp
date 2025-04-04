#include<Arduino.h>

const int ledPin = 5;  // Use a safe GPIO like 5

void setup() {
  Serial.begin(115200);
  ledcSetup(0, 5000, 8);          // channel 0, 5kHz, 8-bit resolution
  ledcAttachPin(ledPin, 0);       // attach ledPin to channel 0
}

void loop() {
  for (int i = 0; i < 255; i++) {
    ledcWrite(0, i);              // write PWM value to channel 0
    delay(10);
  }
  Serial.println("Max Brightness reached");

  for (int i = 254; i >= 0; i--) {
    ledcWrite(0, i);
    delay(10);
  }
  Serial.println("Minimum brightness reached");
  delay(100);
}
