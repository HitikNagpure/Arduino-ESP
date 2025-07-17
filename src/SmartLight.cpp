#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<esp_sleep.h>


#define BLYNK_TEMPLATE_ID "TMPL3x9S7BJOM"
#define BLYNK_TEMPLATE_NAME "Smart Room Light"
#define BLYNK_AUTH_TOKEN "wbYP3LACXb9NMrjhb-XP1KM8iLX-eLv8"

#include<BlynkSimpleEsp32.h>



// Pin Definitions
#define IR_PIN        15
#define TRIG_PIN      13
#define ECHO_PIN      12
#define LED_PIN       4
#define BUZZER_PIN    25


char ssid[] = "Airtel_hrit_9689";
char pass[] = "asdfghjk";

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Distance threshold in cm
bool manualLEDControl = false;
const int distanceThreshold = 30;

// Time tracking
unsigned long lastMotionTime = 0;
unsigned long currentTime = 0;
unsigned long lastPrintTime = 0; // For 1-second Serial logging


// States
bool buzzerPlayed = false;
bool ledOn = false;

int measureDistance();

void sendToBlynk() {
  int motion = digitalRead(IR_PIN);
  long distance = measureDistance();

  Blynk.virtualWrite(V0, distance);
  Blynk.virtualWrite(V1, motion);

  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm   ");

  lcd.setCursor(0, 1);
  lcd.print("Motion: ");
  lcd.print(motion ? "Yes " : "No  ");
}

// Measure distance with HC-SR04
int measureDistance() {
  long duration;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void setup() {
  Wire.begin(21, 22);       // I2C pins for ESP32
  Serial.begin(115200);
  delay(1000);          

  lcd.init();             
  lcd.backlight();           // Turn on backlight

  lcd.setCursor(0, 0);       // Start at top-left corner
  lcd.print("LCD WORKING!");

  lcd.setCursor(0, 1);       // Second line
  lcd.print("ESP32 + I2C");

  pinMode(IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);


  lcd.setCursor(0, 0);
  lcd.print("Smart Room Light");
  delay(1500);
  lcd.clear();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendToBlynk);
}

void loop() {

  Blynk.run(); // Run Blynk
  timer.run(); // Run Blynk Timer
  currentTime = millis();

  // Read IR and distance
  int irValue = digitalRead(IR_PIN);
  int distance = measureDistance();

  // Display distance on LCD
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm   ");

  // Motion detected (IR sensor active LOW = 0)
  if (irValue == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Motion Detected ");

    // Buzzer only once on motion start
    if (!buzzerPlayed) {
      digitalWrite(BUZZER_PIN, HIGH);
      Blynk.virtualWrite(V3, digitalRead(BUZZER_PIN)); // Update buzzer status
      delay(2000);
      digitalWrite(BUZZER_PIN, LOW);
      Blynk.virtualWrite(V3, digitalRead(BUZZER_PIN)); // Update buzzer status

      buzzerPlayed = true;
    }

    lastMotionTime = currentTime;
    delay(200);

    // LED ON if object is close
    if (!manualLEDControl) {
      if (distance < distanceThreshold) {
      digitalWrite(LED_PIN, HIGH);
      Blynk.virtualWrite(V2, digitalRead(LED_PIN)); // Update LED status
      ledOn = true;
      Blynk.virtualWrite(V2, ledOn); // Sync LED status

    } else {
      digitalWrite(LED_PIN, LOW);
      Blynk.virtualWrite(V2, digitalRead(LED_PIN)); // Update LED status
      ledOn = false;
    }
  }

  } else {
    lcd.setCursor(0, 1);
    lcd.print("No Motion       ");

    // LED off after 30 seconds of no motion
    if (currentTime - lastMotionTime >= 30000) {
      digitalWrite(LED_PIN, LOW);
      ledOn = false;
    }

    buzzerPlayed = false; // reset for next motion event
  }
if (currentTime - lastPrintTime >= 1000) { // Print every 1 second
  lastPrintTime = currentTime;

  // Serial output for debug
  Serial.print("IR: ");
  Serial.print(irValue == 0 ? "Motion" : "No Motion");
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.print(" cm | LED: ");
  Serial.print(ledOn ? "ON" : "OFF");
  Serial.print(" | Time since last motion: ");
  Serial.print((currentTime - lastMotionTime) / 1000);
  Serial.println(" sec");
}
  delay(200);
    // If no motion for long, enter light sleep
  if (irValue == HIGH && (currentTime - lastMotionTime) > 30000) {
    Serial.println("No motion for 30 sec. Entering light sleep...");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sleeping...");
    lcd.noBacklight(); // Turn off backlight to save power

    // Configure wakeup on IR_PIN (must be RTC GPIO)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)IR_PIN, 0);

    delay(100); // Let serial flush
    esp_light_sleep_start();  // Enter light sleep

    // After waking up
    Serial.println("Woke up due to motion!");
    lcd.clear();
    lcd.backlight(); // Turn backlight on after waking up
    lcd.setCursor(0, 0);
    lcd.print("Woke up: Motion!");
    delay(1000); // Give user time to see LCD
  }

}


