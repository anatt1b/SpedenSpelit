#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h> // Arduino core library
#include <avr/io.h> // I/O registers
#include <avr/interrupt.h> // Functions and macros related to interrupts

// Define the pins used for button press interrupts
const byte firstPin = 2; // first pin
const byte lastPin =  5; // last pin

// This variable indicates whether the game has started (true = started, false = not started)
extern bool gameStarted; 

// Initializes buttons and their interrupts
void initButtonsAndButtonInterrupts(void);

// New function to check the game start condition
void checkStartButton(void);


// Define the ISR for the PCINT2_vect Pin Change Interrupt
// ISR headerissä julistaminen voi aiheuttaa konfliktia, kommentoidaan pois toistaiseksi
//ISR(PCINT2_vect); 

#endif