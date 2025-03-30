 #include <Arduino.h>

#define TOUCH_THRESHOLD 50

RTC_DATA_ATTR unsigned int counter = 0;

void callback(){
}

void setup(){
  Serial.begin(9600);
  delay(500);
  Serial.println("ESP32 has started");

  Serial.print("Counter = ");
  Serial.println(counter);
  counter++;

  touchAttachInterrupt(T0, callback, TOUCH_THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();

  Serial.println("Going to deep sleep");
  delay(1000);

  esp_deep_sleep_start();
}

void loop(){

}
