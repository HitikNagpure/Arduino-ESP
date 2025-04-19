//Automatic Garage door opener
#include<Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <Ultrasonic.h>

#define TRIG_PIN 14
#define ECHO_PIN 27

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);
AccelStepper stepper(8, 16, 18, 17, 19); // Adjust these GPIOs

LiquidCrystal_I2C lcd(0x27, 16, 2); // Update I2C address if needed

int threshold = 50;
bool closeState = true;
bool openState = false;

void setup() {
  Wire.begin(21,22);
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Garage door open");
}

void loop() {
  int distance = ultrasonic.read();

  Serial.println(distance);

  if (distance > threshold) {
    if (!closeState) {
      stepper.setSpeed(-20);
      for (int i = 0; i < 100; i++) {
        stepper.runSpeed();
        delay(10);
      }
      stepper.setSpeed(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Goodbye");
      closeState = true;
      openState = false;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("No object     ");
    }
  } else if (distance <= threshold) {
    if (!openState) {
      stepper.setSpeed(20);
      for (int i = 0; i < 100; i++) {
        stepper.runSpeed();
        delay(10);
      }
      stepper.setSpeed(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome Back");
      openState = true;
      closeState = false;
    }
  }
}
