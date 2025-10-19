#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

// Public mapping of logical LED indices [0..3] to MCU pins.
// Defined in leds.cpp. Use this array if you need to reference the
// actual pin numbers in other modules (read-only).
extern const byte ledPins[4];

// Convert an Arduino pin number to the logical LED index (0..3).
// Returns true on success and writes the index to `index`. Returns
// false if the pin is not one of the configured LED pins.
bool pinToIndex(int pin, uint8_t &index);

// Initialize LED hardware. Call once during setup().
void initializeLeds(void);

// Turn on the LED at logical index (0..3). If the index is out of
// range this function does nothing. Calling turnOnLed repeatedly for
// the same index will produce a short pulse to make repeated
// activations visible.
void turnOnLed(uint8_t index);

// Turn off all LEDs and reset the internal last-led tracker.
void clearAllLeds(void);

// Turn on all LEDs (useful for testing / end-of-sequence effects).
void setAllLeds(void);

// Blink all LEDs (diagnostic/attention pattern).
void blinkAllLeds(void);

// show1: animation that displays a 4-bit binary counter up and down.
void show1(void);

// show2: runs a single-LED chase sequence for `rounds` iterations.
// Parameter: rounds - number of full passes over the 4 LEDs.
void show2(int rounds);

#endif