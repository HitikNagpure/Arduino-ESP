#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Based on I2C scanner result

void setup() {
  Wire.begin(21, 22);     // ESP32 default SDA, SCL
  Serial.begin(115200);
  delay(1000);            // Small delay for power stability

  lcd.begin(16, 2);       // Initialize LCD
  lcd.backlight();        // Turn on backlight
  lcd.setCursor(1, 0);    // Column 1, Row 0
  lcd.print("WORKING");
  delay(5000);            // Wait 5 seconds
  lcd.clear();            // Clear the display
}

void loop() {
  // Empty loop
}
