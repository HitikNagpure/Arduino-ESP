#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Use address from I2C scanner (confirmed 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin(21, 22);        // I2C pins for ESP32
  Serial.begin(115200);
  delay(1000);               // Let things settle

  lcd.init();                // Correct: use init() instead of begin()
  lcd.backlight();           // Turn on backlight

  lcd.setCursor(0, 0);       // Start at top-left corner
  lcd.print("LCD WORKING!");

  lcd.setCursor(0, 1);       // Second line
  lcd.print("ESP32 + I2C");
}

void loop() {
  // Nothing here
}





// #include <Wire.h>

// void setup() {
//   Wire.begin(21, 22); // SDA, SCL for ESP32
//   Serial.begin(115200);
//   while (!Serial); // wait for serial monitor
//   Serial.println("Scanning I2C devices...");
// }

// void loop() {
//   byte error, address;
//   int nDevices = 0;

//   for(address = 1; address < 127; address++ ) {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();

//     if (error == 0) {
//       Serial.print("I2C device found at 0x");
//       if (address < 16) Serial.print("0");
//       Serial.print(address, HEX);
//       Serial.println(" !");
//       nDevices++;
//     }
//   }

//   if (nDevices == 0)
//     Serial.println("No I2C devices found");
//   else
//     Serial.println("done");

//   delay(5000);
// }
