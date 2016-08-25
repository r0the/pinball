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

#include "parser.h"
#include "consts.h"

ActionParser::ActionParser(const char* filename, uint8_t* events, uint32_t* actions) :
    _actions(actions),
    _error(false),
    _events(events),
    _file(SD.open(filename, FILE_READ)),
    _line(1),
    _nextActionIndex(1)
{
    for (uint8_t i = 0; i < MAX_ACTIONS; ++i) {
        _actions[i] = 0;
    }

    for (uint8_t i = 0; i < EVENT_COUNT; ++i) {
        _events[i] = 0;
    }
}

bool ActionParser::error() const {
    return _error;
}

void ActionParser::parse() {
    _error = true;
    nextChar();
    while (_current != '\0') {
        // parse a line
        if (_current == '@') {
            // parse an event line
            nextChar();
            int eventId = parseEventId();
            if (eventId == -1) {
                return;
            }

            uint8_t firstActionIndex = _nextActionIndex;
            uint32_t action = parseAction();
            while (action != 0) {
                _actions[_nextActionIndex] = action;
                ++_nextActionIndex;
                action = parseAction();
            }

            // mark all actions but the last with continue flag
            for (uint8_t i = firstActionIndex; i < _nextActionIndex - 1; ++i) {
                _actions[i] |= CONTINUE_FLAG;
            }

            // point the event to the first action
            if (firstActionIndex < _nextActionIndex) {
                _events[eventId] = firstActionIndex;
            }
        }

        // seek end of line
        while (_current != '\0' && _current != '\n') {
            nextChar();
        }

        nextChar();
    }

    _error = false;
    _file.close();
}

int ActionParser::parseEventId() {
    if ('a' <= _current && _current <= 't') {
        int result = _current - 'a';
        nextChar();
        return result;
    }
    else {
        return -1;
    }
}

uint32_t ActionParser::parseAction() {
    // parse space
    if (_current != ' ') {
        return 0;
    }

    nextChar();
    // parse variable
    if (_current < 'a' || _current > 'z') {
        return 0;
    }

    uint8_t var = _current - 'a';
    nextChar();
    uint8_t op = parseOperator();
    if (op == OP_NONE) {
        return 0;
    }

    nextChar();
    uint32_t number = 0;
    while ('0' <= _current && _current <= '9') {
        number = number * 10 + (_current - '0');
        nextChar();
    }

    return (static_cast<uint32_t>(op) << OP_SHIFT) | (static_cast<uint32_t>(var) << VAR_SHIFT) | (number & NUMBER_MASK);
}

void ActionParser::nextChar() {
    if (_file.available()) {
        _current = _file.read();
    }
    else {
        _current = '\0';
    }
}

uint8_t ActionParser::parseOperator() {
    switch (_current) {
        case ':':
            return OP_SET;
        case '+':
            return OP_ADD;
        case '-':
            return OP_SUBTRACT;
        case '=':
            return OP_IF_EQUALS;
        case '<':
            return OP_IF_SMALLER;
        case '>':
            return OP_IF_GREATER;
        default:
            return OP_NONE;
    }
}

bool ActionParser::parseSpace() {
    if (_current == ' ') {
        nextChar();
        return true;
    }

    return false;
}

