/*
 * Copyright (C) 2014 - 2015 by Stefan Rothe
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

#ifndef SURE_DISPLAY_H
#define SURE_DISPLAY_H

#include <Arduino.h>

/**
 * This class provides an easy-to-use interface for a Sure 32x8 LED matrix display.
 */
class SureDisplay {
public:
    SureDisplay(byte chipSelectPin, byte clockPin, byte dataPin);
    void setColumn(byte pos, byte data) const;
private:
    void sendCommand(byte data) const;
    void sendDataMS(byte data, byte length) const;
    void sendDataLS(byte data, byte length) const;

    byte _chipSelectPin;
    byte _clockPin;
    byte _dataPin;
};

#endif

