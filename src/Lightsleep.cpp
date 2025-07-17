#include <Arduino.h>
#include "esp_sleep.h"

// IR Sensor Pin (must support wake-up; GPIO 15 is RTC-capable)
#define IR_PIN 15

unsigned long lastMotionTime = 0;
unsigned long currentTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);

  // Initial print
  Serial.println("System Active");
}

void loop() {
  currentTime = millis();

  int irValue = digitalRead(IR_PIN);  // LOW = motion, HIGH = no motion

  if (irValue == LOW) {
    // Motion detected
    Serial.println("Motion detected!");
    lastMotionTime = currentTime;
    delay(200);  // Small debounce
  } else {
    // No motion
    Serial.println("No motion...");
  }

  // Check if it's been idle for 15 seconds
  if (irValue == HIGH && (currentTime - lastMotionTime > 15000)) {
    Serial.println("No motion for 15 sec. Going to light sleep...");

    // Set wake-up on IR pin (when it goes LOW = motion)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)IR_PIN, 0);

    delay(100);  // Let serial flush
    esp_light_sleep_start();  // Enter light sleep

    // Woke up here
    Serial.println("Woke up from sleep due to motion!");
    lastMotionTime = millis();  // Reset timer after wake-up
  }

  delay(500);  // Adjust as needed
}
