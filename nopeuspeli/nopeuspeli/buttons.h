#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 


#define firstPin 2 
#define lastPin 5 

extern volatile int buttonNumber; // Defined at nopeuspeli.ino
extern bool gameRunning; // Defined at nopeuspeli.ino

extern volatile int8_t  buttonQueue;
extern volatile unsigned long buttonTimestamp;

// Function declaration
void initButtonsAndButtonInterrupts(void);


#endif