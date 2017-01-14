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

#include "vars.h"

#define MAX_BALLS 255
#define MAX_SCORE 99999
#define MAX_VAR 65535

#define VAR_FIRST_PIN 0 // 'a'
#define VAR_LAST_PIN 10 // 'k'
#define VAR_FIRST_COUNTDOWN 11 // 'l'
#define VAR_LAST_COUNTDOWN 16 // 'q'
#define VAR_SCORE 18 // 's'
#define VAR_BALLS 19 // 't'
#define VAR_FIRST_VAR 20 // 'u'
#define VAR_LAST_VAR 25 // 'z'

#define STATE_OPEN 0
#define STATE_CLOSED 20

#define INPUT_DELAY 200

void VarsClass::setup() {
    // 1) initialize i/o pins
    _pinInputMode = 0;
    _pinState = 0;
    for (int i = 0; i < IO_PIN_COUNT; ++i) {
        _pinInputLocks[i] = 0;
        _pinInputStates[i] = STATE_OPEN;
        pinMode(IO_PINS[i], OUTPUT);
    }

    // 2) set all countdowns to zero
    for (uint8_t i = 0; i < COUNTDOWN_COUNT; ++i) {
        _countdowns[i] = 0;
    }

    // 4) set score to zero
    _score = 0;

    // 5) set number of balls to three
    _balls = 0;

    // 6) set all standard variables to zero
    for (uint8_t i = 0; i < VAR_COUNT; ++i) {
        _vars[i] = 0;
    }

    _lastTime = millis();
}

void VarsClass::loop() {
    uint32_t now = millis();
    uint32_t dMillis = now - _lastTime;
    _lastTime = now;
    _events = 0;

    // 1) handle i/o pins configured for input
    for (uint8_t pin = 0; pin < IO_PIN_COUNT; ++pin) {
        // only check pins configured for input
        if (_pinInputMode & (1 << pin)) {
            if (_pinInputStates[pin] == STATE_CLOSED) {
                _pinInputStates[pin] = STATE_OPEN;
                _pinInputLocks[pin] = INPUT_DELAY;
            }
            else {
                if (_pinInputLocks[pin] < dMillis) {
                    _pinInputLocks[pin] = 0;
                }
                else {
                   _pinInputLocks[pin] = _pinInputLocks[pin] - dMillis;
                }
            }

            if (_pinInputLocks[pin] == 0) {
                int state = digitalRead(IO_PINS[pin]);
                if (state == LOW && _pinInputStates[pin] < STATE_CLOSED) {
                    ++_pinInputStates[pin];
                    if (_pinInputStates[pin] == STATE_CLOSED) {
                        _events |= (1 << pin);
                    }
                }

                if (state == HIGH) {
                    _pinInputStates[pin] = STATE_OPEN;
                }
            }
        }
    }

    // 2) handle countdowns
    for (uint8_t i = 0; i < COUNTDOWN_COUNT; ++i) {
        if (_countdowns[i] > 0) {
            if (_countdowns[i] > dMillis) {
                _countdowns[i] -= dMillis;
            }
            else {
                _countdowns[i] = 0;
                _events |= 1 << (VAR_FIRST_COUNTDOWN + i);
            }
        }
    }
}

bool VarsClass::hasEvent(uint8_t eventId) const {
    return _events & (1 << eventId);
}

uint32_t VarsClass::score() const {
    return _score;
}

void VarsClass::set(uint8_t varId, uint32_t value) {
    if (VAR_FIRST_PIN <= varId && varId <= VAR_LAST_PIN) {
        // 1) set i/o pin state
        uint8_t pin = IO_PINS[varId - VAR_FIRST_PIN];
        uint16_t pinMask = 1 << (varId - VAR_FIRST_PIN);
        // only if pin is in output mode
        if (!(_pinInputMode & pinMask)) {
            if (value == 0) {
                digitalWrite(pin, LOW);
                _pinState |= pinMask;
            }
            else {
                digitalWrite(pin, HIGH);
                _pinState &= ~pinMask;
            }
        }
    }
    else if (VAR_FIRST_COUNTDOWN <= varId && varId <= VAR_LAST_COUNTDOWN) {
        // 2) set counter value
        if (value > MAX_VAR) {
            _countdowns[varId - VAR_FIRST_COUNTDOWN] = MAX_VAR;
        }
        else {
            _countdowns[varId - VAR_FIRST_COUNTDOWN] = value;
        }
    }
    else if (varId == VAR_SCORE) {
        // 4) set score
        if (value > MAX_SCORE) {
            _score = MAX_SCORE;
        }
        else {
            _score = value;
        }
    }
    else if (varId == VAR_BALLS) {
        // 5) set number of balls
        if (value > MAX_BALLS) {
            _balls = MAX_BALLS;
        }
        else {
            _balls = value;
        }
    }
    else if (VAR_FIRST_VAR <= varId && varId <= VAR_LAST_VAR) {
        // 6) set value of standard variable
        if (value > MAX_VAR) {
            _vars[varId - VAR_FIRST_VAR] = MAX_VAR;
        }
        else {
            _vars[varId - VAR_FIRST_VAR] = value;
        }
    }
}

void VarsClass::setPinInputMode(uint8_t varId) {
    if (VAR_FIRST_PIN <= varId && varId <= VAR_LAST_PIN) {
        uint8_t pin = IO_PINS[varId - VAR_FIRST_PIN];
        uint16_t pinMask = 1 << (varId - VAR_FIRST_PIN);
        pinMode(pin, INPUT_PULLUP);
        _pinInputMode |= pinMask;
    }
}

void VarsClass::setPinOutputMode(uint8_t varId) {
    if (VAR_FIRST_PIN <= varId && varId <= VAR_LAST_PIN) {
        uint8_t pin = IO_PINS[varId - VAR_FIRST_PIN];
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

uint32_t VarsClass::value(uint8_t varId) const {
    if (VAR_FIRST_PIN <= varId && varId <= VAR_LAST_PIN) {
        // 1) get pin state
        uint16_t pinMask = 1 << (varId - VAR_FIRST_PIN);
        if (_pinState & pinMask) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else if (VAR_FIRST_COUNTDOWN <= varId && varId <= VAR_LAST_COUNTDOWN) {
        // 2) get counter value
        return _countdowns[varId - VAR_FIRST_COUNTDOWN];
    }
    else if (varId == VAR_SCORE) {
        // 4) get score
        return _score;
    }
    else if (varId == VAR_BALLS) {
        // 5) get number of balls
        return _balls;
    }
    else if (VAR_FIRST_VAR <= varId && varId <= VAR_LAST_VAR) {
        // 6) get value of normal variable
        return _vars[varId - VAR_FIRST_VAR];
    }
    else {
        return 0;
    }
}

VarsClass Vars;

