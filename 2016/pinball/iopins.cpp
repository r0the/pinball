/*
 * Copyright (C) 2015 - 2016 by Stefan Rothe
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
 
#include "iopins.h"

#define STATE_OPEN 0
#define STATE_CLOSED 20

#define INPUT_DELAY 200

IoPinsClass::IoPinsClass() {
}

void IoPinsClass::setup() {
    for (int pin = 0; pin < IO_PIN_COUNT; ++pin) {
        _inputLock[pin] = 0;
        _inputStates[pin] = STATE_OPEN;
        pinMode(IO_PINS[pin], INPUT_PULLUP);
    }
}

void IoPinsClass::loop(uint32_t dMillis) {
    for (int pin = 0; pin < IO_PIN_COUNT; ++pin) {
        // only check pins configured for input
        if (!(_config & (1 << pin))) {
            if (_inputStates[pin] == STATE_CLOSED) {
                _inputStates[pin] = STATE_OPEN;
                _inputLock[pin] = INPUT_DELAY;
            }
            else {
                if (_inputLock[pin] < dMillis) {
                    _inputLock[pin] = 0;
                }
                else {
                   _inputLock[pin] = _inputLock[pin] - dMillis; 
                }
            }

            if (_inputLock[pin] == 0) {
                int state = digitalRead(IO_PINS[pin]);
                if (state == LOW && _inputStates[pin] < STATE_CLOSED) {
                    ++_inputStates[pin];
                }

                if (state == HIGH) {
                    _inputStates[pin] = STATE_OPEN;
                }
            }
        }
    }
}

bool IoPinsClass::hasEvent(uint8_t pin) const {
    if (pin < IO_PIN_COUNT) {
        return _inputStates[pin] == STATE_CLOSED;
    }
    else {
        return false;
    }
}

bool IoPinsClass::high(uint8_t pin) const {
}

void IoPinsClass::setHigh(uint8_t pin) {
    if (pin < IO_PIN_COUNT && !(_config & (1 << pin))) {
        pin = IO_PINS[pin];
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
}

void IoPinsClass::setLow(uint8_t pin) {
    if (pin < IO_PIN_COUNT && !(_config & (1 << pin))) {
        pin = IO_PINS[pin];
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

void IoPinsClass::setOutput(uint8_t pin) {
    if (pin < IO_PIN_COUNT) {
        _config |= 1 << pin;
        pinMode(IO_PINS[pin], OUTPUT);
    }
}

IoPinsClass IoPins;

