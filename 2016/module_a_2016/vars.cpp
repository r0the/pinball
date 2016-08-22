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
#include "iopins.h"

#define MAX_BALLS 255
#define MAX_SCORE 99999
#define MAX_VAR 65535

#define VAR_FIRST_PIN 'a'
#define VAR_LAST_PIN 'k'
#define VAR_FIRST_VAR 'l'
#define VAR_LAST_VAR 'q'
#define VAR_SCORE 's'
#define VAR_FIRST_COUNTER 't'
#define VAR_LAST_COUNTER 'y'
#define VAR_BALLS 'z'

void VarsClass::setup() {
    _balls = 0;
    _score = 0;
    for (uint8_t i = 0; i < COUNTER_COUNT; ++i) {
        _counters[i] = 0;
    }

    for (uint8_t i = 0; i < VAR_COUNT; ++i) {
        _vars[i] = 0;
    }
}

void VarsClass::loop(uint32_t dMillis) {
    for (uint8_t i = 0; i < COUNTER_COUNT; ++i) {
        if (_counters[i] > dMillis) {
            _counters[i] -= dMillis;
        }
        else {
            _counters[i] = 0;
        }
    }
}

uint32_t VarsClass::score() const {
    return _score;
}

void VarsClass::set(char var, uint32_t value) {
    if (var == VAR_SCORE) {
        if (value > MAX_SCORE) {
            _score = MAX_SCORE;
        }
        else {
            _score = value;
        }
    }
    else if (var == VAR_BALLS) {
        if (value > MAX_BALLS) {
            _balls = MAX_BALLS;
        }
        else {
            _balls = value;
        }
    }
    else if (VAR_FIRST_VAR <= var && var <= VAR_LAST_VAR) {
        if (value > MAX_VAR) {
            _vars[var - VAR_FIRST_VAR] = MAX_VAR;
        }
        else {
            _vars[var - VAR_FIRST_VAR] = value;
        }
    }
    else if (VAR_FIRST_COUNTER <= var && var <= VAR_LAST_COUNTER) {
        if (value > MAX_VAR) {
            _counters[var - VAR_FIRST_COUNTER] = MAX_VAR;
        }
        else {
            _counters[var - VAR_FIRST_COUNTER] = value;
        }
    }
    else if (VAR_FIRST_PIN <= var && var <= VAR_LAST_PIN) {
        if (value == 0) {
            IoPins.setLow(var - VAR_FIRST_PIN);
        }
        else {
            IoPins.setHigh(var - VAR_FIRST_PIN);
        }
    }
}

uint32_t VarsClass::value(char var) const {
    if (var == VAR_SCORE) {
        return _score;
    }
    else if (var == VAR_BALLS) {
        return _balls;
    }
    else if (VAR_FIRST_VAR <= var && var <= VAR_LAST_VAR) {
        return _vars[var - VAR_FIRST_VAR];
    }
    else if (VAR_FIRST_COUNTER <= var && var <= VAR_LAST_COUNTER) {
        return _counters[var - VAR_FIRST_COUNTER];
    }
    else if (VAR_FIRST_PIN <= var && var <= VAR_LAST_PIN) {
        if (IoPins.high(var - VAR_FIRST_PIN)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

VarsClass Vars;

