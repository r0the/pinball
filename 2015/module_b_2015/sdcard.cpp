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

const char event_in01[] PROGMEM = "in01";
const char event_in02[] PROGMEM = "in02";
const char event_in03[] PROGMEM = "in03";
const char event_in04[] PROGMEM = "in04";
const char event_in05[] PROGMEM = "in05";
const char event_in06[] PROGMEM = "in06";
const char event_in07[] PROGMEM = "in07";
const char event_in08[] PROGMEM = "in08";
const char event_in09[] PROGMEM = "in09";
const char event_init[] PROGMEM = "init";
const char event_high[] PROGMEM = "high";
const char event_over[] PROGMEM = "over";

#define EVENT_COUNT 12

// Event names
const char* const eventName[EVENT_COUNT] PROGMEM = {
  event_in01, event_in02, event_in03, event_in04, event_in05, event_in06,
  event_in07, event_in08, event_in09, event_init, event_high, event_over
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

SdCard::SdCard(byte sdChipSelectPin, byte speakerPin) {
    _error = 0;
    if (!SD.begin(sdChipSelectPin)) {
        _error = ERR_SD_CARD_NOT_READY;
    }

    _events = new Event[EVENT_COUNT];
    for (byte id = 0; id < EVENT_COUNT; ++id) {
        init(id);
    }

    loadHighScore();
    _audio.speakerPin = speakerPin;
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

void SdCard::play(byte id) {
    if (_events[id].actions & ACTION_SOUND == ACTION_SOUND) {
        strcpy_P(_filename, (char*) pgm_read_word(&(eventName[id])));
        _filename[4] = '.';
        _filename[5] = 'w';
        _filename[6] = 'a';
        _filename[7] = 'v';
        _filename[8] = '\0';
        if (_audio.isPlaying()) {
            _audio.stopPlayback();
        }
    
        _audio.play(_filename);
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

