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
#include <SD.h>
#include "iopins.h"

#define EVENT_IN_A 0
#define EVENT_IN_B 1
#define EVENT_IN_C 2
#define EVENT_IN_D 3
#define EVENT_IN_E 4
#define EVENT_IN_F 5
#define EVENT_IN_G 6
#define EVENT_IN_H 7
#define EVENT_IN_I 8
#define EVENT_IN_J 9
#define EVENT_IN_K 10
#define EVENT_RESET 11
#define EVENT_HIGH_SCORE 12
#define EVENT_TIMER_T 13
#define EVENT_TIMER_U 14
#define EVENT_TIMER_V 15
#define EVENT_TIMER_W 16
#define EVENT_TIMER_X 17
#define EVENT_TIMER_Y 18
#define EVENT_GAME_OVER 19
#define EVENT_COUNT (EVENT_GAME_OVER + 1)

#define ACTION_SET_HIGH 0
#define ACTION_SET_LOW

#define MAX_ACTIONS 100

/*
 * Actions are internally encoded as a 32-bit unsigned integer.
 *
 * | Bit   | Count | Meaning           |
 * | ----- | ----- | ----------------- |
 * | 31-29 | 3     | Operation         |
 * | 28-24 | 5     | Variable          |
 * | 23-0  | 24    | Argument (Number) |
 *
 */

#define OP_NONE        0 // 000
#define OP_SET         1 // 001
#define OP_ADD         2 // 010
#define OP_SUBTRACT    3 // 011
#define OP_IF_EQUALS   4 // 100
#define OP_IF_SMALLER  5 // 101
#define OP_IF_GREATER  6 // 110
                         // 111

// ----------------------------------------------------------------------------
// class Logic
// ----------------------------------------------------------------------------

class Logic {
public:
    Logic();
    void setup();
    void handleEvent(uint8_t eventId);
private:
    Logic(const Logic&);
    Logic& operator=(const Logic&);
    void executeAction(uint32_t action);
    void opAdd(char var, uint32_t number);
    bool opEquals(char var, uint32_t number) const;
    bool opGreaterThan(char var, uint32_t number) const;
    void opSet(char var, uint32_t number);
    bool opSmallerThan(char var, uint32_t number) const;
    void opSubtract(char var, uint32_t number);
    uint8_t _events[EVENT_COUNT];
    uint32_t _timers[6];
    uint32_t _vars[6];
    uint32_t _actions[MAX_ACTIONS];
    uint8_t _balls;
    uint32_t _score;
};

