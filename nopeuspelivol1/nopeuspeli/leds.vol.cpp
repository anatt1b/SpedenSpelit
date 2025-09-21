#include "leds.vol.h" 


const byte ledPins[] = {A1, A2, A3, A4};  
byte lastLed = 255;  

void initializeLeds() {
    
    for (byte i = 0; i < 4; i++) {
        pinMode(ledPins[i], OUTPUT);  
        digitalWrite(ledPins[i], LOW);  
    }
}


void setLed(byte ledNumber) {
    
    if (lastLed != 255 && lastLed != ledNumber) {
        digitalWrite(ledPins[lastLed], LOW);  
    }
    
    digitalWrite(ledPins[ledNumber], HIGH);
    lastLed = ledNumber;  
}


void clearAllLeds() {
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);  
    }
    lastLed = 255;  
}


void setAllLeds() {
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);  
    }
}


void show1() {
   
    for (int i = 0; i < 16; i++) {
        for (byte j = 0; j < 4; j++) {
            if (i & (1 << j)) {
                digitalWrite(ledPins[j], HIGH);  
            } else {
                digitalWrite(ledPins[j], LOW);   
            }
        }
        delay(500);   
    }
}


void show2(int rounds) {
   
    int delayTime = 500;  
    for (int r = 0; r < rounds; r++) {
        for (byte i = 0; i < 4; i++) {
            clearAllLeds();  
            digitalWrite(ledPins[i], HIGH);  
            delay(delayTime); 
        }
        
        if (delayTime > 100) {
            delayTime -= 50;  
        }
    }
}