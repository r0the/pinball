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
 
#include "sure_display.h"

// Modes
#define MODE_CMD B100
#define MODE_WRITE B101
#define MODE_READ B110
#define MODE_LENGTH 3

// Commands
#define CMD_SYS_DIS B00000000
#define CMD_COM_NMOS_8 B00100000
#define CMD_SYS_EN B00000001
#define CMD_LED_OFF B00000010
#define CMD_LED_ON B00000011
#define CMD_BLINK_OFF B00001000
#define CMD_BLINK_ON B00001001
#define CMD_PWM B10100000

#define COLUMN_COUNT 32

SureDisplay::SureDisplay(byte chipSelectPin, byte clockPin, byte dataPin) {
    _chipSelectPin = chipSelectPin;
    _dataPin = dataPin;
    _clockPin = clockPin;

    // Set pins to output
    pinMode(_chipSelectPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);

    digitalWrite(_chipSelectPin, HIGH);
    digitalWrite(_chipSelectPin, LOW);
    sendDataMS(MODE_CMD, MODE_LENGTH);
    sendCommand(CMD_SYS_DIS);
    sendCommand(CMD_COM_NMOS_8);
    sendCommand(CMD_SYS_EN);
    sendCommand(CMD_LED_ON);
    sendCommand(CMD_PWM | 3);
    digitalWrite(_chipSelectPin, HIGH);
}

void SureDisplay::setColumn(byte pos, byte data) const {
    digitalWrite(_chipSelectPin, LOW);
    sendDataMS(MODE_WRITE, MODE_LENGTH);
    // Write address
    sendDataMS(2 * pos, 7);
    sendDataLS(data, 8);
    digitalWrite(_chipSelectPin, HIGH);
}

void SureDisplay::sendCommand(byte data) const {
    sendDataMS(data, 8);
    digitalWrite(_dataPin, LOW);
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_clockPin, LOW);
}

void SureDisplay::sendDataMS(byte data, byte length) const {
    char mask = 1 << (length - 1);
    for (int i = length - 1; i >= 0; --i) {
        if (data & mask) {
            digitalWrite(_dataPin, HIGH);
        }
        else {
            digitalWrite(_dataPin, LOW);
        }

        digitalWrite(_clockPin, HIGH);
        digitalWrite(_clockPin, LOW);
        mask = mask >> 1;
    }
}

void SureDisplay::sendDataLS(byte data, byte length) const {
    for (int i = 0; i < length; ++i) {
        if (data & 1) {
            digitalWrite(_dataPin, HIGH);
        }
        else {
            digitalWrite(_dataPin, LOW);
        }

        digitalWrite(_clockPin, HIGH);
        digitalWrite(_clockPin, LOW);
        data >>= 1;
    }
}

