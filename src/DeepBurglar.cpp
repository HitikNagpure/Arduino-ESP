#include <Arduino.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>
#include "driver/rtc_io.h"

Ultrasonic ultrasonic(5, 18); // Trig, Echo
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address, cols, rows

const int buzzerPin = 13;
const int irPin = 14;
const int distThresh = 25;

String lastMsg = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 is running!");

  // Power saving: Turn off Wi-Fi and Bluetooth
  btStop();

  Wire.begin(21, 22); // SDA, SCL

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  pinMode(irPin, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Check if this wakeup was from IR sensor (external wake)
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Woke up from IR sensor!");

    int dist = ultrasonic.read();
    bool irDetect = digitalRead(irPin);
    String msg;

    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.print(" cm | IR: ");
    Serial.println(irDetect);

    if ((dist > 0 && dist < distThresh) || irDetect == 0) {
      digitalWrite(buzzerPin, HIGH);
      msg = "Intruder Detected";
    } else {
      digitalWrite(buzzerPin, LOW);
      msg = "No Intruder";
    }

    lcd.setCursor(0, 0);
    lcd.print(msg);

    lcd.setCursor(0, 1);
    lcd.print("Dist:");
    lcd.print(dist);
    lcd.print("cm   ");

    delay(5000); // Give user time to read + sound buzzer
    digitalWrite(buzzerPin, LOW);
    lcd.clear();
    lcd.noBacklight(); // Save power
  }

  // Enable wakeup on IR sensor (GPIO 14, active LOW)
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_14, 0);

  Serial.println("Going to sleep...");
  delay(200); // Ensure messages are printed before sleeping
  esp_deep_sleep_start();
}

void loop() {
  // Will never be reached in deep sleep mode
}
