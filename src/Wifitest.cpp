#include <WiFi.h>

String ssid = "";
String password = "";
bool connecting = false;
unsigned long startAttemptTime;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nEnter WiFi SSID:");
}

void loop() {
  if (ssid == "") {
    if (Serial.available()) {
      ssid = Serial.readStringUntil('\n');
      ssid.trim();
      Serial.print("SSID received: ");
      Serial.println(ssid);
      Serial.println("Enter WiFi Password:");
    }
  } else if (password == "") {
    if (Serial.available()) {
      password = Serial.readStringUntil('\n');
      password.trim();
      Serial.print("Password received: ");
      Serial.println(password);
      Serial.println("Connecting to WiFi...");

      WiFi.begin(ssid.c_str(), password.c_str());
      connecting = true;
      startAttemptTime = millis();
    }
  } else if (connecting) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n✅ WiFi connected!");
      Serial.print("📡 IP address: ");
      Serial.println(WiFi.localIP());
      connecting = false;
    } else if (millis() - startAttemptTime > 15000) {
      Serial.println("\n❌ Failed to connect. Please reset and try again.");
      connecting = false;
    } else {
      Serial.print(".");
      delay(500);
    }
  }
}
