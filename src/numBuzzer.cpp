#include<Arduino.h>
//Control buzzer based on number entered.

// main.cpp

const int buzzerPin = 2;  // You can change this to 4, 16, 17, etc.
bool buzzerState = false;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);  // ESP32 default baud rate is 115200
  digitalWrite(buzzerPin, LOW);
  Serial.println("ESP32 Buzzer Control");
  Serial.println("Enter a number: Odd → ON | Even → OFF");
}

void loop() {
  // Apply the current state to the buzzer
  digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);

  // Check for incoming serial data
  if (Serial.available() > 0) {
    int input = Serial.parseInt();  // Reads the full integer

    if (input % 2 == 0) {
      buzzerState = false;
      Serial.println("Even number → Buzzer OFF");
    } else {
      buzzerState = true;
      Serial.println("Odd number → Buzzer ON");
    }

    // Clear the serial buffer
    while (Serial.available()) Serial.read();
  }

  delay(10);  // Small delay to ease CPU
}
