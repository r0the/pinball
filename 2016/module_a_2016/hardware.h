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

#define PIN_SD_CHIP_SELECT 1
#define PIN_PWM 10

#define PIN_DISPLAY_LATCH_CLOCK 7
#define PIN_DISPLAY_DATA 8
#define PIN_DISPLAY_SHIFT_CLOCK 9

#define IO_PIN_COUNT 11

const uint8_t IO_PINS[IO_PIN_COUNT] = {
    2, 3, 4, 5, 6, A5, A4, A3, A2, A1, A0
};

