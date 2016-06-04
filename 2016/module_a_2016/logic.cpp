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

const char EVENT_NAMES[] PROGMEM =
    "ia" "ib" "ic" "id" "ie" "if" "ig" "ih" "ii" "ij" "ik"
    "on" "hi";


Action::Action(uint8_t command, uint32_t argument) :
    _argument(argument),
    _command(command),
    _next(NULL) {
}

Logic::Logic() :
    _events(new ActionPtr[EVENT_COUNT]) {
}

void Logic::handleEvent(uint8_t eventId) {
    if (eventId >= EVENT_COUNT) {
        return;
    }

    Action* action = _events[eventId];
    while (action != NULL) {
        executeAction(action);
        action = action->next();
    }
}

void Logic::executeAction(Action* action) {
    switch (action->command()) {
        case CMD_ADD_SCORE:
            _score += action->argument();
            break;
        case CMD_SET_SCORE:
            _score = action->argument();
            break;
        case CMD_SUB_SCORE:
            _score -= action->argument();
            break;
        case CMD_ADD_BALL:
            if (_balls < 0xFF) {
                ++_balls;
            }

            break;
        case CMD_SET_BALLS:
            _balls = action->argument() & 0xFF;
            break;
        case CMD_SUB_BALL:
            if (_balls > 0) {
                --_balls;
            }
            break;
        case CMD_OUTPUT_LOW:
            _pins.setLow(action->argument());
            break;
        case CMD_OUTPUT_HIGH:
            _pins.setHigh(action->argument());
            break;
    }
}

