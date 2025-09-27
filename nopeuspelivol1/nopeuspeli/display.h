#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>  // Tarvitaan tyyppien ja pinMode/digitalWrite-funktioiden vuoksi

// --- Konfiguraatiot ---
#define DISPLAY_RESET_PIN     12
#define DISPLAY_SHIFT_CLOCK   11
#define DISPLAY_LATCH_CLOCK   10
#define DISPLAY_OUT_ENABLE     9
#define DISPLAY_SERIAL_INPUT   8

// --- Funktioiden esittelyt ---

/**
 * Alustaa näytön ohjauspinnit.
 */
void initializeDisplay(void);

/**
 * Kirjoittaa yhden numeron 7-segmenttinäytölle.
 * @param number Numero 0–9
 * @param last Jos true, tiedot siirretään näytölle heti
 */
void writeByte(uint8_t number, bool last);

/**
 * Kirjoittaa kaksi numeroa kahdelle näytölle.
 * @param tens Kymmenet
 * @param ones Ykköset
 */
void writeHighAndLowNumber(uint8_t tens, uint8_t ones);

/**
 * Näyttää kokonaisluvun 0–99 kahdella näytöllä.
 * @param result Luku 0–99
 */
void showResult(uint8_t result);

#endif // DISPLAY_H
