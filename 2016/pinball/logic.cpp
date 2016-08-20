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

#define OP_SHIFT 29
#define OP_MASK 7
#define VAR_SHIFT 24
#define VAR_MASK 31
#define NUMBER_MASK 0x0FFF

#define MAX_BALLS 255
#define MAX_SCORE 99999

const char* const EVENT_NAMES PROGMEM = "abcdefghijkrstuvwxyz";

const char* ACTIONS_TXT = "../pinball/actions.txt";

// ----------------------------------------------------------------------------
// class ActionParser
// ----------------------------------------------------------------------------

class ActionParser {
public:
    ActionParser(const char* filename, uint8_t* events, uint32_t* actions) :
        _actions(actions),
        _error(false),
        _events(events),
        _file(SD.open(filename, FILE_READ)),
        _line(1),
        _nextActionIndex(0)
    {
        for (uint8_t i = 0; i < EVENT_COUNT; ++i) {
            _events[i] = 255;
        }
    }

    inline bool error() const {
        return _error;
    }

    inline int line() const {
        return _line;
    }

    void parse() {
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

                _events[eventId] = _nextActionIndex;
                uint32_t action = parseAction();
                while (action != 0) {
                    _actions[_nextActionIndex] = action;
                    ++_nextActionIndex;
                    action = parseAction();
                }

                _actions[_nextActionIndex] = 0;
                ++_nextActionIndex;
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

private:
    uint32_t* _actions;
    char _current;
    bool _error;
    uint8_t* _events;
    File _file;
    int _line;
    uint8_t _nextActionIndex;

    int parseEventId() {
        for (int i = 0; i < EVENT_COUNT; ++i) {
            if (EVENT_NAMES[i] == _current) {
                nextChar();
                return i;
            }
        }

        return -1;
    }

    uint32_t parseAction() {
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

        return (static_cast<uint32_t>(op) << OP_SHIFT) | (var << VAR_SHIFT) | (number & NUMBER_MASK);
    }

    void nextChar() {
        if (_file.available()) {
            _current = _file.read();
        }
        else {
            _current = '\0';
        }
    }

    uint8_t parseOperator() {
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

    bool parseSpace() {
        if (_current == ' ') {
            nextChar();
            return true;
        }

        return false;
    }
};

// ----------------------------------------------------------------------------
// class Logic
// ----------------------------------------------------------------------------

inline void printAction(uint8_t index, uint32_t action) {
    uint8_t op = (action >> OP_SHIFT) & OP_MASK;
    char var = 'a' + ((action >> VAR_SHIFT) & VAR_MASK);
    uint32_t number = (action & NUMBER_MASK);
    char opChar = '*';
    switch (op) {
        case OP_SET: opChar = ':'; break;
        case OP_ADD: opChar = '+'; break;
        case OP_SUBTRACT: opChar = '-'; break;
        case OP_IF_EQUALS: opChar = '='; break;
        case OP_IF_SMALLER: opChar = '<'; break;
        case OP_IF_GREATER: opChar = '>'; break;
    }

    if (opChar == '*') {
        qDebug() << "action[" << index << "] null";
    }
    else {
        qDebug() << "action[" << index << "]" << var << opChar << number;
    }
}


Logic::Logic() :
    _events() {
}

void Logic::setup() {
    ActionParser parser(ACTIONS_TXT, _events, _actions);
    parser.parse();
    if (parser.error()) {
        // TODO
    }
}

void Logic::handleEvent(uint8_t eventId) {
    if (eventId >= EVENT_COUNT) {
        return;
    }

    uint8_t actionIndex = _events[eventId];
    bool loop = true;
    while (loop) {
        uint8_t op = (_actions[actionIndex] >> OP_SHIFT) & OP_MASK;
        char var = 'a' + (_actions[actionIndex] >> VAR_SHIFT) & VAR_MASK;
        uint32_t number = (_actions[actionIndex] & NUMBER_MASK);
        switch (op) {
            case OP_SET:
                opSet(var, number);
                break;
            case OP_ADD:
                opAdd(var, number);
                break;
            case OP_SUBTRACT:
                opSubtract(var, number);
                break;
            case OP_IF_EQUALS:
                if (!opEquals(var, number)) {
                    ++actionIndex;
                }

                break;
            case OP_IF_SMALLER:
                if (!opSmallerThan(var, number)) {
                    ++actionIndex;
                }

                break;
            case OP_IF_GREATER:
                if (!opGreaterThan(var, number)) {
                    ++actionIndex;
                }

                break;
            case OP_NONE:
                loop = false;
                break;
        }

        ++actionIndex;
    }
}

void Logic::opAdd(char var, uint32_t number) {
    if (var == 's') {
        if (_score + number > MAX_SCORE) {
            _score = MAX_SCORE;
        }
        else {
            _score += number;
        }
    }
    else if (var == 'z') {
        if (_balls + number > MAX_BALLS) {
            _balls = MAX_BALLS;
        }
        else {
            _balls += number;
        }
    }
    else if ('l' <= var && var <= 'q') {
        _vars[var - 'l'] += number;
    }
    else if ('t' <= var && var <= 'y') {
        _timers[var - 't'] += number;
    }
}

bool Logic::opEquals(char var, uint32_t number) const {

}

bool Logic::opGreaterThan(char var, uint32_t number) const {

}

void Logic::opSet(char var, uint32_t number) {
    if (var == 's') {
        if (number > MAX_SCORE) {
            _score = MAX_SCORE;
        }
        else {
            _score = number;
        }
    }
    else if (var == 'z') {
        if (number > MAX_BALLS) {
            _balls = MAX_BALLS;
        }
        else {
            _balls = number;
        }
    }
    else if ('l' <= var && var <= 'q') {
        _vars[var - 'l'] = number;
    }
    else if ('t' <= var && var <= 'y') {
        _timers[var - 't'] = number;
    }
}

bool Logic::opSmallerThan(char var, uint32_t number) const {

}

void Logic::opSubtract(char var, uint32_t number) {
    if (var == 's') {
        if (_score < number) {
            _score = 0;
        }
        else {
            _score -= number;
        }
    }
    else if (var == 'z') {
        if (_balls < number) {
            _balls = 0;
        }
        else {
            _balls -= number;
        }
    }
    else if ('l' <= var && var <= 'q') {
        _vars[var - 'l'] -= number;
    }
    else if ('t' <= var && var <= 'y') {
        _timers[var - 't'] -= number;
    }
}
