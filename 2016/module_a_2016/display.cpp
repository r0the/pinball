/*
 * Copyright (C) 2016 by Stefan Rothe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY); without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "display.h"
#include "hardware.h"

static const uint8_t DIGIT[] PROGMEM = {
//    gfedcba
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111  // 9
};

static const uint8_t LETTER[] PROGMEM = {
//   .gfedcba
    B01110111, // A
    B01111100, // B (small)
    B00111001, // C
    B01011110, // D (small)
    B01111001, // E
    B01110001, // F
    B00111101, // G
    B01110110, // H
    B00110000, // I
    B00001110, // J
    B00000000, // K (empty)
    B00111000, // L
    B00000000, // M (empty)
    B01010100, // N (small)
    B00111111, // O
    B01110011, // P
    B00000000, // Q (empty)
    B01010000, // R (small)
    B01101101, // S
    B01111000, // T (small)
    B00111110, // U
    B00000000, // V (empty)
    B00000000, // W (empty)
    B00000000, // X (empty)
    B01110010, // Y (empty)
    B01011011, // Z (empty)
};

#define DISPLAY_CHAR_COUNT 5

static uint8_t lookupChar(char ch) {
    if (ch == ' ' || ch == '\0') {
        return 0;
    }
    else if ('0' <= ch && ch <= '9') {
        return pgm_read_byte_near(DIGIT + ch - '0');
    }
    else if ('A' <= ch && ch <= 'Z') {
        return pgm_read_byte_near(LETTER + ch - 'A');
    }
}

void Display::setup() {
    pinMode(PIN_DISPLAY_LATCH_CLOCK, OUTPUT);
    pinMode(PIN_DISPLAY_DATA, OUTPUT);
    pinMode(PIN_DISPLAY_SHIFT_CLOCK, OUTPUT);
}

void Display::showNumber(unsigned long number) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    unsigned long d = 10000;
    for (int i = 0; i < DISPLAY_CHAR_COUNT; ++i) {
        shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_SHIFT_CLOCK, MSBFIRST,
            pgm_read_byte_near(DIGIT + (number / d) % 10));
        d /= 10;
    }

    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

void Display::showText(const char* text) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    const char* pos = text;
    for (int i = 0; i < DISPLAY_CHAR_COUNT; ++i) {
        shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_SHIFT_CLOCK, MSBFIRST,
            lookupChar(*pos));
        if (*pos != '\0') {
            ++pos;
        }
    }

    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

