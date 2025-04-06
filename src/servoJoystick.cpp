//Controlling servomotor using joystick.
#include<Arduino.h>
#include <ESP32Servo.h>

Servo myservo;
int servoPin = 12;  // Use a PWM-capable pin
int joyX = 34;      // ADC1 pin recommended for analogRead

void setup() {
  myservo.setPeriodHertz(50);         // Standard servo frequency
  myservo.attach(servoPin, 500, 2400); // Min and max pulse width in microseconds
  Serial.begin(9600);
  Serial.println("Move the Joystick to control the servo angle");
}

void loop() {
  int xPos = analogRead(joyX); // 0 to 4095 for ESP32
  int angle = map(xPos, 0, 4095, 0, 180);
  myservo.write(angle);

  Serial.print("Joystick: ");
  Serial.print(xPos);
  Serial.print(" | Angle: ");
  Serial.println(angle);

  delay(10);
}
