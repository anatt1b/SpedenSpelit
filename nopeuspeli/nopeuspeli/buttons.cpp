#include "buttons.h"
#include "nopeuspeli.h" 
static volatile uint8_t g_pd_prev = 0;      //previous state of the port 
volatile int8_t  buttonQueue = -1;          // One button event queue
volatile unsigned long buttonTimestamp = 0; // Timestamp of the button push

static volatile uint8_t pressedMask = 0;                 // PD2..PD5: 1 = button locked down
static volatile unsigned long lastRelease[4] = {0,0,0,0}; // last release for index 0..3
static const unsigned long rearmDelay = 15;              // ms rearmdelay for fall detection

void initButtonsAndButtonInterrupts() {
  pinMode(2, INPUT_PULLUP); pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP); pinMode(5, INPUT_PULLUP);
  cli();
  g_pd_prev = PIND;
  PCICR  |= (1<<PCIE2);
  PCMSK2 |= (1<<PCINT18)|(1<<PCINT19)|(1<<PCINT20)|(1<<PCINT21);
  PCIFR  |= (1<<PCIF2);
  sei();
}

ISR(PCINT2_vect) {
uint8_t pd_now  = PIND;//Reads the whole PORTD at once. Faster then digitalRead()

  const uint8_t m = (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5);//Mask for the bits relevant to this program

  uint8_t changed = (pd_now ^ g_pd_prev) & m;//Using mask to detect changed bits

  uint8_t falling = (~pd_now) & changed;      // Detection of HIGH -> LOW change of bits

  uint8_t rising  = ( pd_now) & changed;      //Detection of LOW  -> HIGH change of bits

  unsigned long now = millis();               // Timestamp for debouncing

  int8_t idx = -1;//Variable reset to -1

//Falling edge detections LSB is read first and MSB read last, lowest bit counts
  if      (falling & (1<<PD2)) 
          idx = 0;
  else if (falling & (1<<PD3)) 
          idx = 1;
  else if (falling & (1<<PD4)) 
          idx = 2;
  else if (falling & (1<<PD5)) 
          idx = 3;

  // one-shot and re-arm accept Falling only if:
  //Button is not locked and since last release rearmDelay has passed
  if (idx >= 0) {
    bool locked = pressedMask & (1 << (idx + 2));            // PD2..PD5 bits
    bool armed  = !locked && (now - lastRelease[idx] >= rearmDelay);
    if (armed && buttonQueue == -1) {
      buttonQueue     = idx;        // One event in que. Is this even necessary?
      buttonTimestamp = now;
      pressedMask    |= (1 << (idx + 2));  // Lock the button to pressed state
    }
  }

  // Unlocking of buttons to be detected
  if (rising & (1<<PD2)) { 
      pressedMask &= ~(1<<PD2); lastRelease[0] = now; 
      }//Releases lock after rising edge and button cleared

  if (rising & (1<<PD3)) {
     pressedMask &= ~(1<<PD3); lastRelease[1] = now; 
     }//Releases lock after rising edge and button cleared

  if (rising & (1<<PD4)) {
     pressedMask &= ~(1<<PD4); lastRelease[2] = now; 
     }//Releases lock after rising edge and button cleared

  if (rising & (1<<PD5)) {
     pressedMask &= ~(1<<PD5); lastRelease[3] = now; 
     }//Releases lock after rising edge and button cleared

  g_pd_prev = pd_now; //update previous state of the port  

}