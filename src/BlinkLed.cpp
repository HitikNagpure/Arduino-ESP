#include <Arduino.h>

void setup(){
    Serial.begin(115200);
    pinMode(2,OUTPUT);
    digitalWrite(2, LOW);
}

void loop(){
    digitalWrite(2,HIGH);
    delay(200);
    digitalWrite(2,LOW);
    delay(200);
}