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

// TODO
// - Highscore event: TEST
// - Game over
// - Game over event

#include <SD.h>
#include "display.h"
#include "audio.h"
#include "logic.h"
#include "vars.h"

#define VERSION 2

#define MODE_TEST 1
#define MODE_GAME 2

uint8_t mode;

const char* TEST_WAV = "test.wav";

void setup() {
    Audio.setup();
    Display.setup();
    Vars.setup();

    if (!SD.begin(PIN_SD_CHIP_SELECT)) {
        Display.show(TEXT_HELLO);
        delay(1000);
        Display.showVersion(VERSION);
        delay(3000);
        while (!SD.begin(PIN_SD_CHIP_SELECT)) {
            Display.show(TEXT_INSRT);
            delay(1000);
            Display.show(TEXT_SDCRD);
            delay(1000);
        }
    }

    if (SD.exists(TEST_WAV)) {
        Audio.play(TEST_WAV);
        Display.show(TEXT_SDRDY);
        delay(1000);
        Display.showNumber(88888);
        delay(3000);
        mode = MODE_TEST;
        // set all i/o pins to input mode
        for (uint8_t eventId = EVENT_IN_A; eventId <= EVENT_IN_K; ++eventId) {
            Vars.setPinInputMode(eventId);
        }    
    }
    else {
        Logic.setup();
        mode = MODE_GAME;
    }
}

void loopTest() {
    for (uint8_t eventId = EVENT_IN_A; eventId <= EVENT_IN_K; ++eventId) {
        if (Vars.hasEvent(eventId)) {
            Display.showPin(eventId);
        }
    }    
}

void loop() {
    Vars.loop();
    switch (mode) {
        case MODE_GAME:
            Logic.loop();
            break;
        case MODE_TEST:
            loopTest();
            break;
    }
}

