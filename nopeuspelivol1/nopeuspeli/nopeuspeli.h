#ifndef NOPEUSPELI_H
#define NOPEUSPELI_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "buttons.vol.h"
#include "leds.vol.h"
#include "display.h"


// GAME BASIC SETTINGS
#define SEQ_LEN 250     // led steps
#define START_HZ 1      // starting frequency
#define HOLD_TIME 2000  // hold button for 2sec to start playing

// BLINK ACCELERATION AFTER EVERY 10 BLINKS +10%
#define ACCEL_EVERY 10      // accelerate every 10th blink
#define ACCEL_PCT 10        // +10% acceleration for blinking

extern volatile bool newTimerInterrupt;      // Timer 1 reports "next led"
extern volatile buttonNumber;                // from buttons.vol.cpp (D2..D5; -1 if not pressed)

// GAME MODE VARIABLES
extern uint8_t seq[SEQ_LEN];        // randomlist for 0-3
extern uint8_t seqIndex;           // next led index 0...SEQ_LEN -1
extern uint8_t currentHZ           // current frequenzy

// randomize 250 LED numbers 0..3
void generateSequence(void);

// blink all leds N times (indicates game is starting)
void blinkAllLeds(void);

// Initialize all game variables before start
void initializeGame(void);

// Start game: Initialize, makes 3x blinks and starts Timer1
void startTheGame(void);

// Timer1: sets desired frequenzy HZ (start lvl)
void setTimer1FrequencyHz(uint8_t hz);

// Timer1 CTC-intterrupt enabled
void initializeTimer(void);

// Increase speed +10% every 10th blinks
void increaseSpeed10pct(void);

// TIMER1_COMPA interrupt (indicates when its time to blink next led)
ISR(TIMER1_COMPA_vect);