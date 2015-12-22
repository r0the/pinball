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
    for (int i = 0; i < _count; ++i) {
        _inputLock[i] = 0;
        _inputStates[i] = 0;
        pinMode(_pins[i], INPUT_PULLUP);
    }
}

void Pins::loop() {
    unsigned long now = millis();
    for (int i = 0; i < _count; ++i) {
        if (_inputLock[i] < now) {
            int state = digitalRead(_pins[i]);
            if (state == HIGH) {
                _inputStates[i] = 0;
            }
            else {
                if (_inputStates[i] < _inputDelay) {
                    ++_inputStates[i];
                }
                else {
                    _inputStates[i] = 0;
                }
            }
        }
    }
}

