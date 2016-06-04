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

#include "sdcard.h"
#include "hardware.h"
#include <SD.h>

#define FIRST_PORT 'a'
#define LAST_PORT 'k'

const char* IO_TXT = "io.txt";

const char event_io_a[] PROGMEM = "io-a";
const char event_io_b[] PROGMEM = "io-b";
const char event_io_c[] PROGMEM = "io-c";
const char event_io_d[] PROGMEM = "io-d";
const char event_io_e[] PROGMEM = "io-e";
const char event_io_f[] PROGMEM = "io-f";
const char event_io_g[] PROGMEM = "io-g";
const char event_io_h[] PROGMEM = "io-h";
const char event_io_i[] PROGMEM = "io-i";
const char event_io_j[] PROGMEM = "io-j";
const char event_io_k[] PROGMEM = "io-k";
const char event_init[] PROGMEM = "init";
const char event_high[] PROGMEM = "high";
const char event_over[] PROGMEM = "over";

#define EVENT_COUNT 13

// Event names
const char* const eventName[EVENT_COUNT] PROGMEM = {
  event_io_a, event_io_b, event_io_c, event_io_d, event_io_e, event_io_f,
  event_io_g, event_io_i, event_io_j, event_io_k, event_init, event_high, event_over
};

#define ACTION_SOUND 1
#define ACTION_TEXT 2
#define ACTION_LOSE_BALL 4
#define ACTION_EXTRABALL 8
#define ACTION_SCORE 16

#define PARSE_COMMAND 0
#define PARSE_NUMBER 1

class Event {
public:
    byte actions;
    unsigned long number;
};

void SdCard::setup() {
    if (SD.begin(PIN_SD_CHIP_SELECT)) {
        _status = SD_READY;
    }
    else {
        _status = SD_NOT_PRESENT;
    }
}

Event* SdCard::lookupPort(char ch) {
    if (FIRST_PORT <= ch && ch <= LAST_PORT) {
        return &_events[ch - FIRST_PORT];
    }
    else {
        return NULL;
    }
}

void SdCard::loadConfig() {
    if (!SD.exists(IO_TXT)) {
        return;
    }

    File file = SD.open(IO_TXT, FILE_READ);
    byte col = 0;
    bool ok = true;
    char ch = '\0';
    char event = '\0';
    while (file.available()) {
        ch = file.read();
        if (ch == '\n') {
            col = 0;
            event = '\0';
        }
        else {
            ++col;
        }
    }

    file.close();
}

const char* SdCard::filename(byte id) {
    strcpy_P(_filename, (char*) pgm_read_word(&(eventName[id])));
    _filename[4] = '.';
    _filename[5] = 't';
    _filename[6] = 'x';
    _filename[7] = 't';
    _filename[8] = '\0';
    return _filename;
}

bool SdCard::hasAction(byte id, byte action) const {
    return (_events[id].actions & action) == action;
}

unsigned long SdCard::number(byte id) const {
    return _events[id].number;
}
void SdCard::init(byte id) {
    _events[id].actions = 0;
    _events[id].number = 0;
    strcpy_P(_filename, (char*) pgm_read_word(&(eventName[id])));
    _filename[4] = '.';
    _filename[5] = 'w';
    _filename[6] = 'a';
    _filename[7] = 'v';
    _filename[8] = '\0';
    if (SD.exists(_filename)) {
        _events[id].actions |= ACTION_SOUND;
    }

    _filename[5] = 't';
    _filename[6] = 'x';
    _filename[7] = 't';
    if (SD.exists(_filename)) {
        _events[id].actions |= ACTION_TEXT;
    }

    _filename[5] = 'p';
    _filename[6] = 'r';
    _filename[7] = 'g';
    if (SD.exists(_filename)) {
        loadProgram(&_events[id]);
    }
}

void SdCard::saveHighScore(unsigned long highScore) {
    _highScore = highScore;
    File file = SD.open("hiscore.dat", FILE_WRITE);
    while (highScore > 0) {
        file.write('0' + (highScore % 10));
        highScore = highScore / 10;
    }

    file.flush();
    file.close();
}

void SdCard::loadHighScore() {
    _highScore = 0;
    File file = SD.open("hiscore.dat", FILE_READ);
    while (file.available()) {
        char ch = file.read();
        if ('0' <= ch && ch <= '9') {
            _highScore = _highScore * 10 + (ch - '0');
        }
        else {
            break;
        }
    }

    file.close();
}

void SdCard::loadProgram(Event* event) {
    File file = SD.open(_filename, FILE_READ);
    if (!file) {
        _error = ERR_FILE_READ;
        return;
    }

    byte mode = PARSE_COMMAND;
    char command = '\0';
    char ch = '\0';
    while (file.available()) {
        ch = file.read();
        switch (ch) {
        case '+':
            event->actions |= ACTION_SCORE;
            mode = PARSE_NUMBER;
            break;
        case 'b':
            event->actions |= ACTION_LOSE_BALL;
            break;
        case 'x':
            event->actions |= ACTION_EXTRABALL;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (mode == PARSE_NUMBER) {
                event->number = event->number * 10 + (ch - '0');
            }

            break;
        default:
            mode = PARSE_COMMAND;
            break;
        }
    }

    file.close();
}

