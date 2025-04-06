//Control speed of stepper motor using joystick
#include<Arduino.h>
#include <AccelStepper.h>

#define joyX 32
#define joyY 33

AccelStepper stepper(AccelStepper::FULL4WIRE, 2, 4, 3, 5); // adjust pins if needed

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Stepper with Joystick");

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(100);
  stepper.setSpeed(200);
  stepper.moveTo(20000);
}

void loop() {
  int xVal = analogRead(joyX);
  int yVal = analogRead(joyY);

  if (xVal < 1000 && yVal > 3000) {
    stepper.setSpeed(200);
  } else if (xVal > 3000 && yVal > 3000) {
    stepper.setSpeed(400);
  } else if (xVal > 3000 && yVal > 1000) {
    stepper.setSpeed(600);
  } else if (xVal < 1000 && yVal < 1000) {
    stepper.setSpeed(800);
  } else {
    stepper.setSpeed(100);
  }

  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(-stepper.currentPosition());
  }

  stepper.run();
}
