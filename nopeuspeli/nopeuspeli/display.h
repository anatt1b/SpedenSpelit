#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>  // Required for types and pinMode/digitalWrite functions

// --- Configuration ---
// Pin definitions for controlling the 7-segment display
#define DISPLAY_RESET_PIN     12 // Pin to reset the display (active HIGH)
#define DISPLAY_SHIFT_CLOCK   11 // Pin for shift clock signal
#define DISPLAY_LATCH_CLOCK   10 // Pin for latch clock signal
#define DISPLAY_OUT_ENABLE     9 // Pin to enable the display (active LOW)
#define DISPLAY_SERIAL_INPUT   8 // Pin for serial data input

// --- Function Declarations ---

/**
 * Initializes the control pins for the display.
 * This function sets the pin modes and prepares the display for use.
 */
void initializeDisplay(void);

/**
 * Writes a single digit to the 7-segment display.
 * @param number The digit to display (0–9).
 * @param last If true, the data is immediately latched to the display.
 */
void writeByte(uint8_t number, bool last);

/**
 * Writes two digits to two separate 7-segment displays.
 * @param tens The tens digit to display (0–9).
 * @param ones The ones digit to display (0–9).
 */
void writeHighAndLowNumber(uint8_t tens, uint8_t ones);

/**
 * Displays a two-digit number (0–99) across two 7-segment displays.
 * @param result The number to display (0–99).
 */
void showResult(uint8_t result);

#endif // DISPLAY_H
