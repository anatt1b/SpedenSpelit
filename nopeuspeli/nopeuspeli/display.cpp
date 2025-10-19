#include "display.h"

// Segment codes for numbers 0–9 (bits: a–g)
const uint8_t segmentCode[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

// Initialize the display by setting up the necessary pins and resetting the display
void initializeDisplay(void) {
  pinMode(DISPLAY_RESET_PIN, OUTPUT);
  pinMode(DISPLAY_SHIFT_CLOCK, OUTPUT);
  pinMode(DISPLAY_LATCH_CLOCK, OUTPUT);
  pinMode(DISPLAY_OUT_ENABLE, OUTPUT);
  pinMode(DISPLAY_SERIAL_INPUT, OUTPUT);

  // Reset and activate the display
  digitalWrite(DISPLAY_RESET_PIN, LOW);
  delay(10);
  digitalWrite(DISPLAY_RESET_PIN, HIGH);
  digitalWrite(DISPLAY_OUT_ENABLE, LOW); // Enable display
}

// Write a single byte to the display
// Parameters:
// - number: The number to display (0–9)
// - last: Whether this is the last byte in the sequence (latches the data if true)
void writeByte(uint8_t number, bool last) {
  uint8_t value = segmentCode[number];

  for (int i = 7; i >= 0; i--) {
    digitalWrite(DISPLAY_SHIFT_CLOCK, LOW);
    digitalWrite(DISPLAY_SERIAL_INPUT, (value >> i) & 0x01);
    digitalWrite(DISPLAY_SHIFT_CLOCK, HIGH);
  }

  if (last) {
    digitalWrite(DISPLAY_LATCH_CLOCK, LOW);
    digitalWrite(DISPLAY_LATCH_CLOCK, HIGH);
  }
}

// Write two numbers to the display (tens and ones place)
// Parameters:
// - tens: The tens digit to display (0–9)
// - ones: The ones digit to display (0–9)
void writeHighAndLowNumber(uint8_t tens, uint8_t ones) {
  writeByte(tens, false); // First display
  writeByte(ones, true);  // Second display 
}

// Display a two-digit result on the display
// Parameters:
// - result: The number to display (0–99)
void showResult(uint8_t result) {
  uint8_t tens = result / 10;
  uint8_t ones = result % 10;
  writeHighAndLowNumber(tens, ones);
}
