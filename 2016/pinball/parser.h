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

#ifndef PARSER_H
#define PARSER_H

#include <Arduino.h>
#include <SD.h>

class ActionParser {
public:
    ActionParser(const char* filename, uint8_t* events, uint32_t* actions);
    bool error() const;
    void parse();
private:
    int parseEventId();
    uint32_t parseAction();
    void nextChar();
    uint8_t parseOperator();
    bool parseSpace();

    uint32_t* _actions;
    char _current;
    bool _error;
    uint8_t* _events;
    File _file;
    int _line;
    uint8_t _nextActionIndex;
};

#endif
