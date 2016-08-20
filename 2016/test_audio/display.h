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

#define TEXT_HELLO 0x1E6DB517
#define TEXT_INSRT 0x23862758
#define TEXT_SDCRD 0x138524E2
#define TEXT_SDRDY 0x284E14E2

#include <Arduino.h>

class Display {
public:
    void setup();
    void show(uint32_t message);
    void showNumber(uint32_t number);
    void showPin(uint8_t pin);
};

#endif

