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

#ifndef DISPLAY_H
#define DISPLAY_H

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

#define TEXT_HELLO 0x1E6DB517
#define TEXT_INSRT 0x23862758
#define TEXT_SDCRD 0x138524E2
#define TEXT_SDRDY 0x284E14E2

#define ERROR_PARSE 1

#include <Arduino.h>

class DisplayClass {
public:
    void setup();
    void show(uint32_t message);
    void showError(uint8_t error);
    void showNumber(uint32_t number);
    void showPin(uint8_t pin);
    void showVersion(uint8_t version);
};

extern DisplayClass Display;

#endif

