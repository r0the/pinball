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

#include "logic.h"
#include "audio.h"
#include "display.h"
#include "parser.h"
#include "vars.h"
#include <SD.h>

#define HIGHSCORE_FLAG 0x01
#define GAMEOVER_FLAG  0x02

// ----------------------------------------------------------------------------
// class Logic
// ----------------------------------------------------------------------------

void LogicClass::setup() {
    _displayLock = 0;
    _filename[0] = 'p';
    _filename[1] = '.';
    _filename[2] = 't';
    _filename[3] = 'x';
    _filename[4] = 't';
    _filename[5] = '\0';
    ActionParser parser(_filename, _events, _actions);
    parser.parse();
    if (parser.error()) {
        Display.showError(ERROR_PARSE);
    }

    // configure i/o pins
    for (uint8_t varId = 0; varId < 11; ++varId) {
        if (_actions[_events[varId]] == 0) {
            Vars.setPinOutputMode(varId);
        }
        else {
            Vars.setPinInputMode(varId);
        }
    }

    // read existing high score
    _filename[0] = 's';
    File f = SD.open(_filename, FILE_READ);
    while (f.available()) {
        char c = f.read();
        if ('0' <= c && c <= '9') {
            _highscore *= 10;
            _highscore += c - '0';
        }
    }

    f.close();

    // check for existing sound files
    _sounds = 0;
    _filename[2] = 'w';
    _filename[3] = 'a';
    _filename[4] = 'v';
    for (uint8_t eventId = 0; eventId < EVENT_COUNT; ++eventId) {
        _filename[0] = 'a' + eventId;
        if (SD.exists(_filename)) {
            _sounds |= 1 << eventId;
        }
    }

    _flags = 0;
    handleEvent(EVENT_RESET);
}

uint32_t count = 0;

void LogicClass::loop() {
    uint32_t now = millis();
    for (int i = 0; i < EVENT_COUNT; ++i) {
        if (Vars.hasEvent(i)) {
            handleEvent(i);
        }
    }

    if (_flags & HIGHSCORE_FLAG) {
        // new highscore has been reached, highscore variable now counts down to next auto save
        if (_highscore < now) {
            _highscore = now + HIGHSCORE_SAVE_TIMEOUT;
            saveHighscore();
        }
    }
    else {
        // no new highscore has been reached yet, check if it's the case now
        if (Vars.score() > _highscore) {
            handleEvent(EVENT_HIGHSCORE);
            Display.show(TEXT_HISCR);
            _displayLock = now + DISPLAY_TEXT_TIMEOUT;
            _flags |= HIGHSCORE_FLAG;
            _highscore = now + HIGHSCORE_SAVE_TIMEOUT;
            saveHighscore();
        }
    }

    if (_displayLock < now) {
        Display.showNumber(Vars.score());
    }

    // check for game over
}

void LogicClass::handleEvent(uint8_t eventId) {
    if (eventId >= EVENT_COUNT) {
        return;
    }

    if (_sounds & (1 << eventId)) {
        _filename[0] = 'a' + eventId;
        Audio.play(_filename);
    }

    uint8_t actionIndex = _events[eventId];
    bool continueFlag = true;
    while (continueFlag) {
        continueFlag = _actions[actionIndex] & CONTINUE_FLAG;
        uint8_t op = (_actions[actionIndex] >> OP_SHIFT) & OP_MASK;
        uint8_t varId = (_actions[actionIndex] >> VAR_SHIFT) & VAR_MASK;
        uint32_t number = (_actions[actionIndex] & NUMBER_MASK);
        switch (op) {
            case OP_SET:
                Vars.set(varId, number);
                break;
            case OP_ADD:
                Vars.set(varId, Vars.value(varId) + number);
                break;
            case OP_SUBTRACT:
                Vars.set(varId, Vars.value(varId) - number);
                break;
            case OP_IF_EQUALS:
                if (Vars.value(varId) != number) {
                    ++actionIndex;
                    continueFlag = _actions[actionIndex] & CONTINUE_FLAG;
                }

                break;
            case OP_IF_SMALLER:
                if (Vars.value(varId) >= number) {
                    ++actionIndex;
                    continueFlag = _actions[actionIndex] & CONTINUE_FLAG;
                }

                break;
            case OP_IF_GREATER:
                if (Vars.value(varId) <= number) {
                    ++actionIndex;
                    continueFlag = _actions[actionIndex] & CONTINUE_FLAG;
                }

                break;
        }

        ++actionIndex;
    }
}

void LogicClass::saveHighscore() {
    _filename[0] = 's';
    _filename[2] = 't';
    _filename[3] = 'x';
    _filename[4] = 't';
    File f = SD.open(_filename, O_READ | O_WRITE | O_CREAT | O_TRUNC);
    uint32_t score = Vars.score();
    f.write('0' + (score / 10000) % 10);
    f.write('0' + (score / 1000) % 10);
    f.write('0' + (score / 100) % 10);
    f.write('0' + (score / 10) % 10);
    f.write('0' + score % 10);
    f.println("");
    f.close();
    _filename[2] = 'w';
    _filename[3] = 'a';
    _filename[4] = 'v';
}

LogicClass Logic;

