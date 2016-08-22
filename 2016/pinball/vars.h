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

#ifndef VARS_H
#define VARS_H

#include <Arduino.h>

#define COUNTER_COUNT 6
#define VAR_COUNT 6

class VarsClass {
public:
    void setup();
    void loop(uint32_t dMillis);
    void add(char var, uint32_t value);
    uint32_t score() const;
    void set(char var, uint32_t value);
    void subtract(char var, uint32_t value);
    uint32_t value(char var) const;
private:
    uint8_t _balls;
    uint32_t _counters[COUNTER_COUNT];
    uint32_t _score;
    uint32_t _vars[VAR_COUNT];
};

extern VarsClass Vars;

#endif

