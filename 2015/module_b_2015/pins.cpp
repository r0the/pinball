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
 
#define DEFAULT_INPUT_DELAY 50

Pins::Pins(byte count, const byte* pins) :
    _count(count),
    _inputDelay(DEFAULT_INPUT_DELAY),
    _inputLock(new unsigned long[count]),
    _inputStates(new byte[count]),
    _pins(pins)
{
    for (int pin = 0; pin < _count; ++pin) {
        _inputLock[pin] = 0;
        _inputStates[pin] = 0;
        pinMode(_pins[pin], INPUT_PULLUP);
    }
}

void Pins::loop() {
    unsigned long now = millis();
    for (int pin = 0; pin < _count; ++pin) {
        if (_inputLock[pin] < now) {
            int state = digitalRead(_pins[pin]);
            if (state == HIGH) {
                _inputStates[pin] = 0;
            }
            else {
                if (_inputStates[pin] < _inputDelay) {
                    ++_inputStates[pin];
                }
                else {
                    _inputStates[pin] = 0;
                }
            }
        }
    }
}

