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

// ----------------------------------------------------------------------------
// class Logic
// ----------------------------------------------------------------------------

void LogicClass::setup() {
    _filename[0] = 'p';
    _filename[1] = '.';
    _filename[2] = 't';
    _filename[3] = 'x';
    _filename[4] = 't';
    ActionParser parser(_filename, _events, _actions);
    parser.parse();
    if (parser.error()) {
        Display.showError(ERROR_PARSE);
    }

    // check for i/o events
    for (uint8_t eventId = 0; eventId < EVENT_COUNT; ++eventId) {
        if (_actions[_events[eventId]] != 0) {
            Vars.setPinInputMode(eventId);
        }
    }

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

    handleEvent(EVENT_RESET);
}

void LogicClass::loop() {
    for (int i = 0; i < EVENT_COUNT; ++i) {
        if (Vars.hasEvent(i)) {
            handleEvent(i);
        }
    }

    // check for new highscore
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

    Display.showNumber(Vars.score());
}

LogicClass Logic;

