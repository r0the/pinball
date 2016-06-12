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

const char* const EVENT_NAMES PROGMEM = "abcdefghijkrstuvwxyz";

const char* ACTIONS_TXT = "../pinball/actions.txt";

// ----------------------------------------------------------------------------
// class ActionParser
// ----------------------------------------------------------------------------

class ActionParser {
  public:
    ActionParser(const char* filename) :
      _error(false),
      _file(SD.open(filename, FILE_READ)),
      _line(1) {
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

          while (parseAction()) {

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

  private:
    bool _error;
    File _file;
    int _line;
    char _current;

    int parseEventId() {
      for (int i = 0; i < EVENT_COUNT; ++i) {
        if (EVENT_NAMES[i] == _current) {
          nextChar();
          return i;
        }
      }

      return -1;
    }

    bool parseAction() {
      // parse space
      if (_current != ' ') {
        return false;
      }

      nextChar();
      // parse variable
      if (_current < 'a' || _current > 'z') {
        return false;
      }

      uint8_t var = _current - 'a';
      nextChar();
      uint8_t op = 0;
      switch (_current) {
        case ':':
          op = OP_ASSIGN;
          break;
        case '+':
          op = OP_ADD;
          break;
        case '-':
          op = OP_SUBTRACT;
          break;
        case '=':
          op = OP_IF_EQUALS;
          break;
        case '<':
          op = OP_IF_SMALLER;
          break;
        case '>':
          op = OP_IF_GREATER;
          break;
        default:
          return false;
      }

      nextChar();
      uint32_t number = 0;
      while ('0' <= _current && _current <= '9') {
        number = number * 10 + (_current - '0');
        nextChar();
      }

      return true;
    }

    void nextChar() {
      if (_file.available()) {
        _current = _file.read();
      }
      else {
        _current = '\0';
      }
    }

    int parseOperator() {
      switch (_current) {
        case ':':
          return OP_ASSIGN;
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
          return -1;
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

Logic::Logic() :
  _events() {
}

void Logic::setup() {
  ActionParser parser(ACTIONS_TXT);
  parser.parse();
  if (parser.error()) {
  }
}

void Logic::handleEvent(uint8_t eventId) {
  if (eventId >= EVENT_COUNT) {
    return;
  }

  //    Event* event = _events[eventId];
  //    while (event != NULL) {
  //        executeEvent(event);
  //        event = event->next;
  //    }
}

void Logic::executeAction(uint32_t action) {
  //    switch (action->command()) {
  //        case CMD_ADD_SCORE:
  //            setScore(_score + action->argument());
  //            break;
  //        case CMD_SET_SCORE:
  //            setScore(action->argument());
  //            break;
  //        case CMD_SUB_SCORE:
  //            setScore(_score - action->argument());
  //            break;
  //        case CMD_ADD_BALL:
  //            if (_balls < 0xFF) {
  //                setBalls(_balls + 1);
  //            }

  //            break;
  //        case CMD_SET_BALLS:
  //            setBalls(action->argument() & 0xFF);
  //            break;
  //        case CMD_SUB_BALL:
  //            if (_balls > 0) {
  //                setBalls(_balls - 1);
  //            }

  //            break;
  //        case CMD_OUTPUT_LOW:
  //            _pins.setLow(action->argument());
  //            break;
  //        case CMD_OUTPUT_HIGH:
  //            _pins.setHigh(action->argument());
  //            break;
  //    }
}

void Logic::executeEvent(uint8_t eventId) {

}

void Logic::setBalls(uint8_t balls) {
  _balls = balls;
}

void Logic::setScore(uint32_t score) {
  _score = score;
}

