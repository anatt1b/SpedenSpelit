#include "nopeuspeli.h"
#include "buttons.h" 
#include "display.h"
#include "leds.h"   
#include <string.h>  // Needed for memset
#include <util/atomic.h>

// ==================== Variables ====================
// Global variables only declared in this file:
volatile bool newTimerInterrupt = false;
bool gameRunning = false;//Game running variable tracks if the game is running
volatile int buttonNumber = -1; // Variable declared here other files need to use external

// Global variables (extern)
uint16_t seq[SEQ_BYTES];             // Käyttää SEQ_BYTES nopeuspeli.h:sta
uint16_t seqIndex = 0;               //Led progression variable. Tracks the led sequence progression
uint8_t flashesSinceAccel = 0;      //Game speed accelleration variable
uint16_t currentLed = 0;             //Current led according to game sequence
uint8_t score = 0;                  //Score variable
uint16_t currentHz = START_HZ;      // Game speed variable setting at game start up
uint8_t gameProgression = 0;        //Variable to track game progression by how many correct button pushes has been made

const uint8_t START_BUTTON_INDEX = 3; // D5


uint16_t defaultTicks = (uint16_t)(F_CPU / (1024UL * START_HZ) - 1);      // Game speed variable setting at game start up. Will be updated every speed up
uint16_t outputCompareTicks = defaultTicks;      // Game speed variable setting at game start up. Will be updated every speed up


// ==================== SETUP ====================
void setup() {
    Serial.begin(9600);
    Serial.println("Setup begins");
    initializeDisplay();
    initializeLeds();
    initButtonsAndButtonInterrupts();
    TIMSK1 &= ~(1 << OCIE1A);//Stops the timer from running before game starts
    randomSeed(analogRead(A0));    
}

// ==================== LED-Sequence generation function) ====================
void generateSequence(void) {
    memset(seq, 0, SEQ_BYTES);  //Initialize array
    for (uint16_t i = 0; i < MAX_LEDS; i++) {
        uint8_t rnd = random(0, 4);
        uint8_t byteIdx = i / 4;
        uint8_t shift = (i % 4) * 2;
        seq[byteIdx] |= (rnd & 0x03) << shift;
    }
}
//Function to read step values from game sequence
uint8_t getLedFromSequence(uint8_t index) {
    uint8_t byteIdx = index / 4;
    uint8_t shift = (index % 4) * 2;
    return (seq[byteIdx] >> shift) & 0x03;
}

// ==================== GAMELOGIC  ====================
    



//Function for setting the timer interrupt frequency
void setTimer1FrequencyHz(uint16_t registerValue) {

    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    // Using prescaler 1024 (CS12 ja CS10)
    // OCR1A-value
    OCR1A = registerValue;

    TCCR1B |= (1 << WGM12);       // CTC-mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
    TIMSK1 |= (1 << OCIE1A);             // Enable Timer1 compare interrupt
    interrupts();

}
//Function that initializes timer
void initializeTimer(void) {
    // This is used to reset OCR1A value to its
    setTimer1FrequencyHz(defaultTicks); 
}
//Function that increases game speed
void increaseSpeed10pct(void) {
    outputCompareTicks = round(outputCompareTicks / 1.1);
    if (outputCompareTicks < 700){
        outputCompareTicks = 700;
    }
    setTimer1FrequencyHz(outputCompareTicks);
    Serial.println(outputCompareTicks);
    Serial.println(seqIndex);

}

//Game initialization function to reset variables etc
void initializeGame(void) {
    clearAllLeds();
    Serial.println("Game starts");
    seqIndex = 0;
    flashesSinceAccel = 0;
    currentHz = START_HZ;
    currentLed = 0;
    score = 0;
    gameProgression = 0;
    showResult(score);
    generateSequence();
}

//Function that starts the game
void startTheGame() {
    initializeGame();
    outputCompareTicks = defaultTicks;
    show1();
    delay(500); // Short break after startup
    newTimerInterrupt = false;//Reset interrupt flaag
    setTimer1FrequencyHz(defaultTicks); // Start from START_HZ
    gameRunning = true;//Update game running flag
}


void checkGame(uint8_t pressedIdx) {

    if (!gameRunning) return;
    uint8_t expected = getLedFromSequence(gameProgression);//Reads the expected button to press from game sequence according to game proggression
            //Prints for debuging the game
            Serial.print("pressed="); Serial.print(pressedIdx);                             //Prints out the pressed button 
            Serial.print(" expected="); Serial.print(getLedFromSequence(gameProgression));  //Prints out the expected button to press
            Serial.print(" gameProgression="); Serial.print(gameProgression);               //Prints Game progression tracking variable
            Serial.print(" currentLed="); Serial.print(currentLed);                         //Prints out current led that is turned on according to sequence
            Serial.print(" seqIndex="); Serial.println(seqIndex);                           //Prints out where the led sequence in going

    // Checking of game progression and sequence:
    if (pressedIdx == expected) {               //Compares pressed button and expected button from game sequence
        if (score < 99) score++;                //Increase the score
        showResult(score);                      //Calls function to show score on display
        gameProgression++;                      //Increases game progression after right button was pressed
    } else {
        noInterrupts();
        TIMSK1 &= ~(1 << OCIE1A);
        interrupts();
        clearAllLeds();
        show2(3);
        gameRunning = false;

    }
}


// ==================== TIMER ISR ====================
ISR(TIMER1_COMPA_vect) {
    newTimerInterrupt = true;
}

// ==================== LED-HANDLER ====================
void handleNewLed(void) {
    if (newTimerInterrupt) {
        newTimerInterrupt = false;
    if (seqIndex <= MAX_LEDS) {//If sequence has gone through the whole array stop lighting leds
            // turnOnLed() handles control of leds on/off
            currentLed = getLedFromSequence(seqIndex);
            turnOnLed(currentLed);

            seqIndex++;
            flashesSinceAccel++;
    }
        if (flashesSinceAccel >= ACCEL_EVERY_FLASHES) { // use constant
           increaseSpeed10pct();
            flashesSinceAccel = 0; // Reset counter
        }

        if (seqIndex >= MAX_LEDS) { // use constant
           clearAllLeds();  // 1024 should be enough to make the player forget the sequence :D
        }
    }
}

// ==================== LOOP ====================
void loop() {
  static unsigned long lastHandled = 0;
  const  unsigned long debounceWin = 40; //Debounce time

  int8_t bn = -1; 
  unsigned long ts = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {           //Disables the interupts to write the snapshot variable
    bn = buttonQueue;                           //Snaphot variable of pressed button
    ts = buttonTimestamp;                       //Timestamp snapshot variable for debouncing
    buttonQueue = -1;                           //Button variable that is written by ISR, only reset here for next interupt
  }

  if (bn >= 0) {//Only runs if button was pressed
    // debounce rejects presses too close to eachother
    if (ts - lastHandled >= debounceWin) {
      lastHandled = ts;

      if (bn == (int)START_BUTTON_INDEX && !gameRunning) {
        startTheGame();                         //Game starts if the left most button is pressed
      } else {
        checkGame((uint8_t)bn);                 //Game checking function that checks the button presses and handles scores etc
      }
    }
  }

  handleNewLed();
}