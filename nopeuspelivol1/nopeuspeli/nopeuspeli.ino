#include <Arduino.h>
#include "leds.vol.h"
#include "buttons.vol.h"
#include "display.h"
#include "nopeuspeli.h"

// LOCAL VARIABLES
static bool gameRunning = false;
static bool holdActive = false;
static unsigned long holdStart = 0;


void setup() {

Serial.begin(9600);
initializeLeds();
initButtonsAndButtonInterrupts();
initializeDisplay();

clearAllLeds();

}

void loop() {
show1();

// 1) starting game by holdin button 1(D2) for 2sec
if(!gameRunning){
  if(digitalRead(2) == LOW){      // button 2 hold
    if(!holdActive){
      holdActive = true;
      holdStart = millis();
    }
    else if(millis() - holdStart >= HOLD_TIME){
      startTheGame();             // initializeGam + 3x blink + enable Timer1
      gameRunning = true;
      holdActive = false;         // clear button 1 hold
    }
  }
  else {
    holdActive = false;           // button 1 released
  }
}

// 2) GAME RUNNING: TIMER INDICATES NEXT LED
  if(gameRunning && newTimerInterrupt){
    newTimerInterrupt = false;

    // light up next led
    clearAllLeds();
    setLed(seq[seqIndex]);

    // acceleration every 10th blink +10%

    if seqIndex > 0 && (seqIndex % ACCEL_EVERY_FLASHES){
      increaseSpeed10pct();
    }

    // move to next led sequence
    seqIndex++;

    // 3) THE END
    if(seqIndex >= SEQ_LEN){
      
      //stop Timer1 COMPA
      noInterrupts();
      TIMSK1 &= ~(1 << OCIE1A);
      interrupts();

      clearAllLeds();
      show2(4);
      gameRunning = false;

    }
  }