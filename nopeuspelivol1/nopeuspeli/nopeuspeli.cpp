#include "nopeuspeli.h"
#include "leds.vol.h"
#include "buttons.h"
#include "display.h"

// GLOBAL GAME VARIABLES
volatile bool newTimerInterrupt = false;    // Timer1 "next led"   
volatile int buttonNumber = -1;             // buttons ISR 

uint8_t seq[SEQ_LEN];               // pre randomized led order 0..3
uint8_t seqIndex = 0;                // sequence tracking
uint8_t currentHz = START_HZ;       // leds start flashing freguency
uint8_t flashesSinceAccel = 0;       // tracking how many flashes from laste acceleration
uint8_t currentLed = 0;              // tracks current led
uint8_t score = 0;                   //  score track

// RAFFLE 250 RANDOM NUMBERS FOR LED FLASHES
void generateSequence(void){
    randomSeed(analogRead(A0));
    for(uint8_t i = 0; i < SEQ_LEN; i++){
        seq[i] = (uint8_t)random(0, 4);
    }
}

// FLASH ALL LEDS 3 TIMES
void blinkAllLeds(void){
    for(uint8_t i = 0; i < 3; i++){
        setAllLeds();
        delay(200);
        clearAllLeds();
        delay(200);
    }
}

// Timer1: set frequenzy Hz(CTC, prescaler 64)
void setTimer1FrequencyHz(uint8_t hz){
    // gap hz
    if(hz < 1) hz = 1;
    if(hz > 255) hz = 255;

    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    ORC1A = (uint16_t)(F_CPU / (64UL * (uint32_t)hz) - 1);

    TCCR1B |= (1 << WGM12);                 // CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10);    // prescaler 64
    TIMSK1 |= (1 << OCIE1A);                // COMPA interrupt enable
    interrupts();

    currentHz = hz;
}

//  TIMER STARTS WITH STARTING FREQUENCY
void initializeTimer(void){
    setTimer1Frequency(START_HZ);
}

// +10% ACCELERATION
void increaseSpeed10pct(void){
    uint8_t inc = (uint8_t)(currentHz + 9) / 10);
    setTimer1FrequencyHz((uint8_t)(currentHz + inc));
}

// INTITIALIZE GAME
void initializeGame(void){
    clearAllLeds();
    seqIndex = 0;
    flashesSinceAccel = 0;
    currentHz = START_HZ;
    currentLed = 0;
    score = 0;
    showResult(score);      // show 00
    generateSequence();     // raffle 250 led numbers
}

// STARTING THE GAME, CALLED WHEN BUTTON1(D2) IS HELD FOR 2S
void startTheGame(void){
    initializeGame();
    blinkAllLeds();         // 3x flashes
    newTimerInterrupt = false;
    initializeTimer();      // Timer1 set ON -> ISR starts "ticking"
}

// CHECK FOR PRESSED BUTTON + SCORE COUNT
void checkGame(byte nbrOfButtonPush){
    uint8_t pressedIdx;
    if(!pinToIndex((int)nbrOfButtonPush, pressedIdx))
    return;

    // correct press (same as led)
    if(pressedIdx == currentLed){
        if(score < 99) score++;
        showResult(score);
    } else{
        // wrong interruput ends game
        noInterrupts();
        TIMSK1 &= ~(1 << OCIE1A)    // Timer1 COMPA disabled
        interrupts();
        clearAllLeds();
        show2(4);
    }
}

// TIMER1 COMPA ISR
ISR(TIMER1_COMPA_vect){
    newTimerInterrupt = true;
}
