#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 

// Käytetään #define const-muuttujien sijaan C-ympäristön takia
#define firstPin 2 
#define lastPin 5 

extern volatile int buttonNumber; // Määritelty nopeuspeli.ino:ssa
extern bool gameRunning; // Korjattu: Käytetään gameRunning-nimeä (Määritelty nopeuspeli.ino:ssa)

extern volatile int8_t  buttonQueue;
extern volatile unsigned long buttonTimestamp;

// Funktioesittelyt
void initButtonsAndButtonInterrupts(void);


#endif