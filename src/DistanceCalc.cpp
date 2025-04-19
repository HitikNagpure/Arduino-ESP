#include <Arduino.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address, cols, rows

void setup() {
  Wire.begin(21,22);
  Serial.begin(115200);
  lcd.begin(16,2 );
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Distance Calc.");
  delay(5000);
  lcd.clear();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration;
  float distance;

  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse
  duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout after 30ms

  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  if (duration == 0) {
    Serial.println("Out of range");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Out of Range");
  } else {
    Serial.print("Distance: ");
    lcd.setCursor(0,1);
    lcd.print("Distance: ");
    Serial.print(distance);
    lcd.print(distance,1);
    Serial.println(" cm");
    lcd.print(" cm");
  }

  delay(2000);
}
