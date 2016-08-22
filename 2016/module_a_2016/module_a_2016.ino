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

uint8_t mode;
uint32_t lastTime;

const char* TEST_WAV = "test.wav";

void setup() {
    Audio.setup();
    Display.setup();
    IoPins.setup();

    if (!SD.begin(PIN_SD_CHIP_SELECT)) {
        Display.show(TEXT_HELLO);
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
    }
    else {
        Logic.setup();
        mode = MODE_GAME;
    }

    lastTime = millis();
}

void loopTest() {
    for (int i = 0; i < IO_PIN_COUNT; ++i) {
        if (IoPins.hasEvent(i)) {
            Display.showPin(i);
        }
    }    
}

void loopGame() {
    Logic.loop();
    for (int i = 0; i < IO_PIN_COUNT; ++i) {
        if (IoPins.hasEvent(i)) {
            Logic.handleEvent(i);
        }
    }
}

void loop() {
    uint32_t now = millis();
    uint32_t dMillis = now - lastTime;
    lastTime = now;
    IoPins.loop(dMillis);
    Vars.loop(dMillis);
    switch (mode) {
        case MODE_GAME:
            loopGame();
            break;
        case MODE_TEST:
            loopTest();
            break;
    }
}

