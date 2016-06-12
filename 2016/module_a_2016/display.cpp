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

#define CHAR_SPACE 10
#define CHAR_MINUS 11
#define CHAR_A     16
#define CHAR_B     17
#define CHAR_C     18
#define CHAR_D     19
#define CHAR_E     20
#define CHAR_F     21
#define CHAR_G     22
#define CHAR_H     23
#define CHAR_I     24
#define CHAR_J     25
#define CHAR_K     26
#define CHAR_L     27
#define CHAR_M     28
#define CHAR_N     29
#define CHAR_O     30
#define CHAR_P     31
#define CHAR_Q     32
#define CHAR_R     33
#define CHAR_S     34
#define CHAR_T     35
#define CHAR_U     36
#define CHAR_V     37
#define CHAR_W     38
#define CHAR_X     39
#define CHAR_Y     40
#define CHAR_Z     41

#define DISPLAY_CHAR_COUNT 5

static void shiftOutCode(uint8_t code) {
    shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_SHIFT_CLOCK, MSBFIRST, 
            pgm_read_byte_near(FONT + code));
}

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

void Display::setup() {
    pinMode(PIN_DISPLAY_LATCH_CLOCK, OUTPUT);
    pinMode(PIN_DISPLAY_DATA, OUTPUT);
    pinMode(PIN_DISPLAY_SHIFT_CLOCK, OUTPUT);
}

void Display::show(uint32_t message) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    uint8_t ch = 0;
    for (uint8_t i = 0; i < DISPLAY_CHAR_COUNT; ++i) {
        shiftOutCode(message & 0x3F);
        message >>= 6;
    }

    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);    
}

void Display::showNumber(uint32_t number) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    uint32_t d = 10000;
    for (int i = 0; i < DISPLAY_CHAR_COUNT; ++i) {
        shiftOutCode((number / d) % 10);
        d /= 10;
    }

    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

void Display::showPin(uint8_t pin) {
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, LOW);
    shiftOutCode(CHAR_I);
    shiftOutCode(CHAR_O);
    shiftOutCode(CHAR_MINUS);
    shiftOutCode(CHAR_A + pin);
    shiftOutCode(CHAR_SPACE);
    digitalWrite(PIN_DISPLAY_LATCH_CLOCK, HIGH);
}

