#include "leds.h" // Header for LED utility functions

// Array mapping logical LED indices [0..3] to microcontroller pins.
// Uses analog pin constants (A1..A4) to match the original wiring.
const byte ledPins[] = {A1, A2, A3, A4};

// Remember the last LED index that was turned on. 255 means "none".
// This is used to briefly toggle the same LED off/on when it is
// requested repeatedly to create a visible refresh effect.
byte lastLed = 255;

// Blink all LEDs on for 200 ms, then off for 200 ms.
// Simple diagnostic / attention pattern.
void blinkAllLeds() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);
    }
    delay(200);
    for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
    }
    delay(200);
}

// Turn on a single LED by logical index (0..3).
// If the same LED is requested twice in a row, briefly turn it off
// and back on (30 ms) to make the repeated activation visible.
// If a different LED is requested, all LEDs are cleared first so the
// requested LED is the only one lit.
// Parameters:
//   index - LED index in range [0,3]. If out of range, the function
//           returns without doing anything.
void turnOnLed(uint8_t index) {
    if (index >= 4) return; // guard against invalid indexes

    if (lastLed == index) {
        // Same LED requested again: pulse it to visible effect
        digitalWrite(ledPins[index], LOW);
        delay(30);  // short gap to make the re-activation visible
        digitalWrite(ledPins[index], HIGH);
    } else {
        // Different LED: clear others and light requested one
        clearAllLeds();
        digitalWrite(ledPins[index], HIGH);
        lastLed = index;
    }
}


// Initialize LED pins as outputs and ensure all LEDs are off.
// Call once during system setup.
void initializeLeds() {
    for (byte i = 0; i < 4; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
}

// Turn off all LEDs and reset lastLed marker.
void clearAllLeds() {
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
    }
    lastLed = 255; // no LED currently recorded as "last"
}

// Turn on all LEDs 
void setAllLeds() {
    for (byte i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);
    }
}

// show1: display a 4-bit binary counter forwards and then backwards.
// Each step shows the 4-bit value on the 4 LEDs (LSB on ledPins[0]).
// Delays control the animation speed; tuned for a visible demo.
void show1() {
    // Count up (0..15)
    for (int i = 0; i < 16; i++) {
        for (byte j = 0; j < 4; j++) {
            digitalWrite(ledPins[j], (i & (1 << j)) ? HIGH : LOW);
        }
        delay(150); // step delay
    }

    delay(250); // pause at the end of the sequence

    // Count down (15..0)
    for (int i = 15; i >= 0; i--) {
        for (byte j = 0; j < 4; j++) {
            digitalWrite(ledPins[j], (i & (1 << j)) ? HIGH : LOW);
        }
        delay(150);
    }
    clearAllLeds();
}

// show2: runs a sequence where LEDs light one by one for `rounds` times.
// The delay between steps shortens over time to create accelerating effect.
// Parameter:
//   rounds - how many full passes of the 4-LED sequence to perform.
void show2(int rounds) {
    int delayTime = 500;

    // Starting flash effect: blink all LEDs 3 times
    for (int i = 0; i < 3; i++) {
        setAllLeds();
        delay(500);
        clearAllLeds();
        delay(500);
    }

    // Main sequence: light each LED in turn, for rounds iterations.
    for (int r = 0; r < rounds; r++) {
        for (byte i = 0; i < 4; i++) {
            clearAllLeds();
            digitalWrite(ledPins[i], HIGH);
            delay(delayTime);
        }

        // Gradually reduce the delay to accelerate the sequence 
        if (delayTime > 100) {
            delayTime -= 50;
        }
    }

    clearAllLeds();
}