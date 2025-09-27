#include "display.h"

// Segmenttikoodit numeroille 0–9 (bitit: a–g)
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

void initializeDisplay(void) {
  pinMode(DISPLAY_RESET_PIN, OUTPUT);
  pinMode(DISPLAY_SHIFT_CLOCK, OUTPUT);
  pinMode(DISPLAY_LATCH_CLOCK, OUTPUT);
  pinMode(DISPLAY_OUT_ENABLE, OUTPUT);
  pinMode(DISPLAY_SERIAL_INPUT, OUTPUT);

  // Reset ja aktivoi näyttö
  digitalWrite(DISPLAY_RESET_PIN, LOW);
  delay(10);
  digitalWrite(DISPLAY_RESET_PIN, HIGH);
  digitalWrite(DISPLAY_OUT_ENABLE, LOW); // Näyttö päälle
}

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

void writeHighAndLowNumber(uint8_t tens, uint8_t ones) {
  writeByte(tens, false); // Ensimmäinen näyttö
  writeByte(ones, true);  // Toinen näyttö
}

void showResult(uint8_t result) {
  uint8_t tens = result / 10;
  uint8_t ones = result % 10;
  writeHighAndLowNumber(tens, ones);
}
