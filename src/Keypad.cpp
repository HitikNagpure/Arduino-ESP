#include<Arduino.h>
#include <Keypad.h>

int ledpin;
void ledpattern1();
void ledpattern2();
void ledpattern3();
void ledpattern4();

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};


byte rowPins[ROWS] = {34, 35, 32, 33}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {25 ,26 , 27, 14}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void setup(){
    Serial.begin(9600);

    for (ledpin =10; ledpin < 14; ledpin++) {
        pinMode(ledpin, OUTPUT);
    }

    Serial.println("Enter_the_pattern_number_(1,2,3,4)_");
}
void loop(){
    char reqdPattern = kpd.getKey();
    if (reqdPattern){
        if (reqdPattern == '1')
        ledpattern1();
        else if (reqdPattern == '2')
        ledpattern2();
        else if (reqdPattern == '3')
        ledpattern3();
        else if (reqdPattern == '4')
        ledpattern4();
        else
        Serial.println("Invalid Pattern");
    }
}
void ledpattern1(){
    for(ledpin = 10; ledpin<14; ledpin++) {
        digitalWrite(ledpin,HIGH);
        delay(1000);
    }
    for (ledpin = 10; ledpin< 14; ledpin++){

        digitalWrite(ledpin,LOW);
        delay(1000);
    }
}
void ledpattern2(){
    for(ledpin = 10; ledpin < 14; ledpin++){
        digitalWrite(ledpin, HIGH);
        delay(1000);
        digitalWrite(ledpin, LOW);
        delay(1000); 
    }
}
void ledpattern3(){
    for(ledpin = 13; ledpin > 10; ledpin--){
        digitalWrite(ledpin, HIGH);
        delay(1000); 
    }
    for(ledpin = 13; ledpin > 10; ledpin--){
        digitalWrite(ledpin, LOW);
        delay(1000);
    }
}
void ledpattern4(){
    for(ledpin = 9; ledpin <14; ledpin = ledpin+2){
        digitalWrite(ledpin, HIGH);
        delay(1000);
    }
    for(ledpin = 10; ledpin <14; ledpin = ledpin+2){
        digitalWrite(ledpin, HIGH);
        delay(1000);
    }
    for (ledpin = 9; ledpin <14; ledpin = ledpin+2) {
        digitalWrite(ledpin, LOW);
        delay(1000); 
    }
    for (ledpin = 10; ledpin <14; ledpin = ledpin+2) {
        digitalWrite(ledpin, LOW);
        delay(1000); 
    }
}
