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

#ifndef LOGIC_H
#define LOGIC_H

#include <Arduino.h>
#include "consts.h"

// ----------------------------------------------------------------------------
// class Logic
// ----------------------------------------------------------------------------

class LogicClass {
public:
    void setup();
    void loop();
    void handleEvent(uint8_t eventId);
private:
    void executeAction(uint32_t action);
    uint32_t _actions[MAX_ACTIONS];
    uint8_t _events[EVENT_COUNT];
    char _filename[5];
    uint32_t _highscore;
    uint32_t _sounds;
};

extern LogicClass Logic;

#endif

