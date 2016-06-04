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

#include <Arduino.h>
#include "iopins.h"

#define EVENT_IA 0
#define EVENT_IB 1
#define EVENT_IC 2
#define EVENT_ID 3
#define EVENT_IE 4
#define EVENT_IF 5
#define EVENT_IG 6
#define EVENT_IH 7
#define EVENT_II 8
#define EVENT_IJ 9
#define EVENT_IK 10
#define EVENT_ON 11
#define EVENT_HI 12
#define EVENT_COUNT (EVENT_HI + 1)

#define CMD_ADD_SCORE     0
#define CMD_SET_SCORE     1
#define CMD_SUB_SCORE     2
#define CMD_ADD_BALL      3
#define CMD_SET_BALLS     4
#define CMD_SUB_BALL      5
#define CMD_COUNTDOWN_1   6
#define CMD_OUTPUT_LOW    7
#define CMD_OUTPUT_HIGH   8

class Action {
public:
    Action(uint8_t command, uint32_t argument);
    inline uint32_t argument() const { return _argument; }
    inline uint8_t command() const { return _command; }
    Action* next() const { return _next; }
private:
    uint8_t _command;
    uint32_t _argument;
    Action* _next;
};

typedef Action* ActionPtr;

class Logic {
public:
    Logic();
    void handleEvent(uint8_t eventId);
private:
    void executeAction(Action* action);
    ActionPtr* _events;
    uint8_t _balls;
    uint32_t _score;
    IoPins _pins;
};

