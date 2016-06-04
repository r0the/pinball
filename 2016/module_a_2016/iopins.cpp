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
#include "hardware.h"


#define CONFIG_INPUT  0x01
#define CONFIG_OUTPUT 0x02

#define STATE_OPEN 0
#define STATE_CLOSED 20

#define INPUT_DELAY 200

IoPins::IoPins() :
    _config(new uint8_t[IO_PIN_COUNT]),
    _inputLock(new uint32_t[IO_PIN_COUNT]),
    _inputStates(new uint8_t[IO_PIN_COUNT]) {
    for (int pin = 0; pin < IO_PIN_COUNT; ++pin) {
        _config[pin] = CONFIG_INPUT;
        _inputLock[pin] = 0;
        _inputStates[pin] = STATE_OPEN;
        pinMode(IO_PINS[pin], INPUT_PULLUP);
    }
}

void IoPins::loop() {
    unsigned long now = millis();
    for (int pin = 0; pin < IO_PIN_COUNT; ++pin) {
        if (_config[pin] & CONFIG_INPUT) {
            if (_inputStates[pin] == STATE_CLOSED) {
                _inputStates[pin] = STATE_OPEN;
                _inputLock[pin] = now + INPUT_DELAY;
            }
            if (_inputLock[pin] < now) {
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

boolean IoPins::hasEvent(uint8_t pin) const {
    if (pin < IO_PIN_COUNT) {
        return _inputStates[pin] == STATE_CLOSED;
    }
    else {
        return false;
    }
}

void IoPins::setHigh(uint8_t pin) {
    if (pin < IO_PIN_COUNT && (_config[pin] & CONFIG_OUTPUT)) {
        pinMode(IO_PINS[pin], OUTPUT);
        digitalWrite(IO_PINS[pin], HIGH);
    }
}

void IoPins::setLow(uint8_t pin) {
    if (pin < IO_PIN_COUNT && (_config[pin] & CONFIG_OUTPUT)) {
        pinMode(IO_PINS[pin], OUTPUT);
        digitalWrite(IO_PINS[pin], LOW);
    }
}

