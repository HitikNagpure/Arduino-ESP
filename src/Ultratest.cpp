#include<Arduino.h>
#include<Wire.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(5, 18); // Trig, Echo

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 is running!");
  Wire.begin(21,22);

}

void loop() {
  Serial.println("Looping...");
  delay(1000);
  
  int dist = ultrasonic.read();
  Serial.println(dist+"cm");
  delay(2000);
}
