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

#include "display.h"
#include "audio.h"
#include "iopins.h"
#include "logic.h"
#include "vars.h"

#define MODE_TEST 1
#define MODE_GAME 2

Display display;
Audio audio;
IoPins pins;
Vars vars(pins);
Logic logic(display, vars);
uint8_t mode;

const char* TEST_WAV = "test.wav";

void setup() {
    audio.begin();
    pins.setup();
    display.setup();

    if (!SD.begin(PIN_SD_CHIP_SELECT)) {
        display.show(TEXT_HELLO);
        delay(3000);
        while (!SD.begin(PIN_SD_CHIP_SELECT)) {
            display.show(TEXT_INSRT);
            delay(1000);
            display.show(TEXT_SDCRD);
            delay(1000);
        }
    }

    if (SD.exists(TEST_WAV)) {
        audio.play(TEST_WAV);
        display.show(TEXT_SDRDY);
        delay(1000);
        display.showNumber(88888);
        delay(3000);
        mode = MODE_TEST;
    }
    else {
        logic.setup();
        mode = MODE_GAME;
    }
}

void loopTest() {
    for (int i = 0; i < IO_PIN_COUNT; ++i) {
        if (pins.hasEvent(i)) {
            display.showPin(i);
        }
    }    
}

void loopGame() {
    logic.loop();
    for (int i = 0; i < IO_PIN_COUNT; ++i) {
        if (pins.hasEvent(i)) {
            logic.handleEvent(i);
        }
    }
}

void loop() {
    pins.loop();
    switch (mode) {
        case MODE_GAME:
            loopGame();
            break;
        case MODE_TEST:
            loopTest();
            break;
    }
}

