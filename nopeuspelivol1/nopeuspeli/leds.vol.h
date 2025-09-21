#ifndef LEDS_H
#define LEDS_H

#include <arduino.h> 

void initializeLeds();           
void setLed(byte ledNumber);     
void clearAllLeds(void);         
void setAllLeds(void);           
void show1(void);                
void show2(int rounds);          

#endif