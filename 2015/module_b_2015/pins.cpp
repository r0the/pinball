/*
 * Copyright (C) 2015 by Stefan Rothe
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
 
#include "pins.h"
 
#define CONFIG_INPUT 0
#define CONFIG_OUTPUT 1

Pins::Pins(byte count, const byte* pins) :
    _config(new byte[count]),
    _count(count),
    _inputLock(new unsigned long[count]),
    _inputStates(new byte[count]),
    _pins(pins)
{
    for (int pin = 0; pin < _count; ++pin) {
        _config[pin] = CONFIG_INPUT;
        _inputLock[pin] = 0;
        _inputStates[pin] = 1;
        pinMode(_pins[pin], INPUT_PULLUP);
    }
}

void Pins::loop() {
    unsigned long now = millis();
    for (int pin = 0; pin < _count; ++pin) {
        // Clear event from last loop
        if (_inputStates[pin] >= INPUT_DELAY) {
          _inputStates[pin] = 0;
        }

        if (_config[pin] == CONFIG_INPUT && _inputLock[pin] < now) {
            int state = digitalRead(_pins[pin]);
            if (state == HIGH) {
                _inputStates[pin] = 1;
            }
            else if (0 < _inputStates[pin] && _inputStates[pin] < INPUT_DELAY) {
                ++_inputStates[pin];
            }
        }
    }
}

void Pins::setHigh(int pin) {
    _config[pin] = CONFIG_OUTPUT;
    pinMode(_pins[pin], OUTPUT);
    digitalWrite(_pins[pin], HIGH);
}

void Pins::setLow(int pin) {
    _config[pin] = CONFIG_OUTPUT;
    pinMode(_pins[pin], OUTPUT);
    digitalWrite(_pins[pin], LOW);
}

