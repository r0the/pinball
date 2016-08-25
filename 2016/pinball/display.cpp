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
#include "consts.h"

#define DISPLAY_CHAR_COUNT 5

#ifndef QT_CORE_LIB

static const uint8_t FONT[] PROGMEM = {
//   .gfedcba
    B00111111, //  0
    B00000110, //  1
    B01011011, //  2
    B01001111, //  3
    B01100110, //  4
    B01101101, //  5
    B01111101, //  6
    B00000111, //  7
    B01111111, //  8
    B01101111, //  9
    B00000000, // 10: Space
    B01000000, // 11: -
    B00000000, // 12: (reserved)
    B00000000, // 13: (reserved)
    B00000000, // 14: (reserved)
    B00000000, // 15: (reserved)
    B01110111, // 16: A
    B01111100, // B (small)
    B00111001, // C
    B01011110, // D (small)
    B01111001, // E
    B01110001, // F
    B00111101, // G
    B01110110, // H
    B00110000, // I
    B00001110, // J
    B01111010, // K
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
    B01011011  // Z (empty)
};

static void shiftOutCode(uint8_t code) {
    shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_SHIFT_CLOCK, MSBFIRST,
             pgm_read_byte_near(FONT + code));
}

#else

uint8_t bufferPos = 0;
char buffer[5];

static void shiftOutCode(uint8_t code) {
    if (code < 10) {
        buffer[bufferPos] = '0' + code;
    }
    else if (code == CHAR_SPACE) {
        buffer[bufferPos] = ' ';
    }
    else if (code == CHAR_MINUS) {
        buffer[bufferPos] = '-';
    }
    else if (CHAR_A <= code && code <= CHAR_Z) {
        buffer[bufferPos] = 'A' + code - CHAR_A;
    }

    ++bufferPos;
    if (bufferPos > 4) {
        qDebug() << "Display:" << buffer;
        bufferPos = 0;
    }
}

#endif

static uint8_t convertChar(char ch) {
    if (ch == '-') {
        return CHAR_MINUS;
    }
    else if ('0' <= ch && ch <= '9') {
        return ch - '0';
    }
    else if ('A' <= ch && ch <= 'Z') {
        return CHAR_A + ch - 'A';
    }
    else {
        return CHAR_SPACE;
    }
}

void DisplayClass::setup() {
    pinMode(PIN_DISPLAY_LATCH_CLOCK, OUTPUT);
    pinMode(PIN_DISPLAY_DATA, OUTPUT);
    pinMode(PIN_DISPLAY_SHIFT_CLOCK, OUTPUT);
}

void DisplayClass::show(uint32_t message) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    uint8_t ch = 0;
    for (uint8_t i = 0; i < DISPLAY_CHAR_COUNT; ++i) {
        shiftOutCode(message & 0x3F);
        message >>= 6;
    }

    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

void DisplayClass::showError(uint8_t error) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    shiftOutCode(CHAR_E);
    shiftOutCode(CHAR_R);
    shiftOutCode(CHAR_R);
    shiftOutCode((error / 10) % 10);
    shiftOutCode(error % 10);
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

void DisplayClass::showNumber(uint32_t number) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    uint32_t d = 10000;
    for (int i = 0; i < DISPLAY_CHAR_COUNT; ++i) {
        shiftOutCode((number / d) % 10);
        d /= 10;
    }

    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

void DisplayClass::showPin(uint8_t pinId) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    shiftOutCode(CHAR_I);
    shiftOutCode(CHAR_O);
    shiftOutCode(CHAR_MINUS);
    shiftOutCode(CHAR_A + pinId);
    shiftOutCode(CHAR_SPACE);
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

void DisplayClass::showVersion(uint8_t version) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    shiftOutCode(CHAR_R);
    shiftOutCode(CHAR_SPACE);
    shiftOutCode(CHAR_SPACE);
    shiftOutCode((version / 10) % 10);
    shiftOutCode(version % 10);
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

DisplayClass Display;

