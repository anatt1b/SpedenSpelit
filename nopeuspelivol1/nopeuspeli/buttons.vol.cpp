#include "buttons.vol.h"

volatile int buttonNumber = -1; // last pressed button
extern bool gameStarted;  //This variable indicates whether the game has started (true = started, false = not started)
unsigned long buttonPressStartTime = 0;   // Store the start time of the button press
bool buttonBeingHeld = false;  // Variable to track if the button is being held down


// DEBOUNCE
unsigned long lastDebounceTime = 0; // Last approved moment of pressing button
const unsigned long debounceDelay = 300;  // 300 milliseconds debounce delay

// BUTTON AND INTERRUPT INITIALIZATION
void initButtonsAndButtonInterrupts(void) {
    // Initialize the game buttons (pins 2-5) as inputs
    for (byte pin = firstPin; pin <= lastPin; pin++) {
        pinMode(pin, INPUT_PULLUP);  // Internal pull-up resistor enabled
    }

    // Enable Pin Change Interrupt for port D (pins 2-5)
    PCICR |= (1 << PCIE2);  // Enable pin change interrupts 
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Enable interrupts
    // for pins 2, 3, 4, and 5 to detect changes
}

// INTERRUPT HANDLING AND DEBOUNCING
ISR(PCINT2_vect) {
    // Check if the debounce time has elapsed
    unsigned long currentTime = millis();

    // 
    if ((currentTime - lastDebounceTime) < debounceDelay) {
        return;
    }
     
    // read buttons one by one
    if(digitalRead(2) == LOW){
        buttonNumber = 2;
    }
    else if(digitalRead(3) == LOW){
        buttonNumber = 3;
    }
    else if(digitalRead(4) == LOW){
        buttonNumber = 4;
    }
    else if(digitalRead(5) == LOW){
        buttonNumber = 5;
    }
    else{
        //BUTTON WAS NOT PRESSED
        return;
    }

    lastDebounceTime = currentTime;
}