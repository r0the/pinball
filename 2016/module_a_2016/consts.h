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

#ifndef CONSTS_H
#define CONSTS_H


#define DISPLAY_TEXT_TIMEOUT 1500
#define HIGHSCORE_SAVE_TIMEOUT 5000

// ----------------------------------------------------------------------------
// pins
// ----------------------------------------------------------------------------

#define PIN_SD_CHIP_SELECT 1
#define PIN_PWM 10

#define PIN_DISPLAY_LATCH_CLOCK 7
#define PIN_DISPLAY_DATA 8
#define PIN_DISPLAY_SHIFT_CLOCK 9

#define IO_PIN_COUNT 11

const uint8_t IO_PINS[IO_PIN_COUNT] = {
    2, 3, 4, 5, 6, A5, A4, A3, A2, A1, A0
};

// ----------------------------------------------------------------------------
// variables
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// events
// ----------------------------------------------------------------------------

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
#define EVENT_COUNTDOWN_L 11
#define EVENT_COUNTDOWN_M 12
#define EVENT_COUNTDOWN_N 13
#define EVENT_COUNTDOWN_O 14
#define EVENT_COUNTDOWN_P 15
#define EVENT_COUNTDOWN_Q 16
#define EVENT_RESET 17
#define EVENT_HIGHSCORE 18
#define EVENT_GAME_OVER 19
#define EVENT_COUNT (EVENT_GAME_OVER + 1)

// ----------------------------------------------------------------------------
// actions
// ----------------------------------------------------------------------------
/*
 * Actions are internally encoded as a 32-bit unsigned integer.
 *
 * | Bit   | Count | Meaning           |
 * | ----- | ----- | ----------------- |
 * | 31    | 1     | Continue          |
 * | 30-27 | 4     | Operation         |
 * | 26-21 | 5     | Variable          |
 * | 20-20 | 2     | Reserved          |
 * | 19-0  | 20    | Argument (Number) |
 *
 */

#define MAX_ACTIONS 64

#define CONTINUE_FLAG 0x8000

#define OP_SHIFT 27
#define OP_MASK 0x000F
#define VAR_SHIFT 21
#define VAR_MASK 31
#define NUMBER_MASK 0x000FFFFF

#define OP_NONE        0 // 0000
#define OP_SET         1 // 0001
#define OP_ADD         2 // 0010
#define OP_SUBTRACT    3 // 0011
#define OP_IF_EQUALS   4 // 0100
#define OP_IF_SMALLER  5 // 0101
#define OP_IF_GREATER  6 // 0110

#endif

