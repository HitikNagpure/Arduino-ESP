#define BLYNK_TEMPLATE_ID "TMPL3QS1AuhO8"
#define BLYNK_TEMPLATE_NAME "Smart Room Light"
#define BLYNK_AUTH_TOKEN "y6j6nfJfXDIZlC6OJWPAp0KFICEdZQrV"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Airtel_hrit_9689";
char pass[] = "asdfghjk";

#define IR_SENSOR_PIN 15 // Connect OUT of IR sensor here

BlynkTimer timer;

void checkMotion() {
  int motion = digitalRead(IR_SENSOR_PIN);
  Blynk.virtualWrite(V0, motion); // Send to app
  Serial.println(motion ? "Motion Detected" : "No Motion");
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_SENSOR_PIN, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, checkMotion); // Check every second
}

void loop() {
  Blynk.run();
  timer.run();
}
