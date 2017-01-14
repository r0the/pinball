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
#include "consts.h"

#define COUNTDOWN_COUNT 6
#define VAR_COUNT 6

class VarsClass {
public:
    void setup();
    void loop();
    bool hasEvent(uint8_t eventId) const;
    uint32_t score() const;
    void set(uint8_t varId, uint32_t value);
    void setPinInputMode(uint8_t varId);
    void setPinOutputMode(uint8_t varId);
    uint32_t value(uint8_t varId) const;
private:
    uint8_t _balls;
    uint16_t _countdowns[COUNTDOWN_COUNT];
    uint16_t _events;
    uint32_t _lastTime;
    uint8_t _pinInputLocks[IO_PIN_COUNT];
    uint16_t _pinInputMode;
    uint8_t _pinInputStates[IO_PIN_COUNT];
    uint16_t _pinState;
    uint32_t _score;
    uint16_t _vars[VAR_COUNT];
};

extern VarsClass Vars;

#endif

