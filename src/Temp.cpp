#include<Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin(21,22);
  Serial.begin(115200);
  lcd.begin(16,2 );
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("DHT11 Sensor");
  dht.begin();
  delay(5000);
  lcd.clear();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String tempM = "Temp: " + String(t) + "C";
  String humM = "Hum: " + String(h) + "%";

  Serial.println(tempM + " " + humM);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(tempM);
  lcd.setCursor(0, 1);
  lcd.print(humM);

  delay(5000);
}
