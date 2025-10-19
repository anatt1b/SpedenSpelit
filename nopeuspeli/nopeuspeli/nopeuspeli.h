#ifndef NOPEUSPELI_H
#define NOPEUSPELI_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// GAME BASIC SETTINGS
#define MAX_LEDS 1024            // led steps
//#define SEQ_BYTES (MAX_LEDS/4)  // bytes needed for 2-bit packed sequence
#define SEQ_BYTES ((MAX_LEDS + 3) / 4)
#define START_HZ 1              // Korjattu: START_HZ määritelty (esim. 2 Hz)
#define HOLD_TIME 2000          // hold button for 2sec to start playing

// BLINK ACCELERATION
#define ACCEL_EVERY_FLASHES 10
#define ACCEL_PCT 10

// GAME MODE VARIABLES
extern volatile bool newTimerInterrupt; 
extern volatile int buttonNumber;      
extern bool gameRunning;

extern uint16_t seq[SEQ_BYTES]; 
extern uint16_t seqIndex;
extern uint16_t currentHz;
extern uint8_t flashesSinceAccel;
extern uint16_t currentLed;
extern uint8_t score; 

// FUNCTION PROTOTYPES (Stays same)
void generateSequence(void);
uint8_t getLedFromSequence(uint8_t index);
void blinkAllLeds(void);
void initializeGame(void);
void startTheGame(void);
void setTimer1FrequencyHz(uint16_t hz);
void initializeTimer(void);
void increaseSpeed10pct(void);
void checkGame(uint8_t pressedIdx);
void handleNewLed(void);

#endif