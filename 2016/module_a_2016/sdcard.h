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

#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>

#define SD_READY 1
#define SD_NOT_PRESENT 2

#define EVENT_IO_A 0
#define EVENT_IO_B 1
#define EVENT_IO_C 2
#define EVENT_IO_D 3
#define EVENT_IO_E 4
#define EVENT_IO_F 5
#define EVENT_IO_G 6
#define EVENT_IO_H 7
#define EVENT_IO_I 8
#define EVENT_IO_J 9
#define EVENT_IO_K 10

#define ACTION_SOUND 1
#define ACTION_TEXT 2
#define ACTION_LOSE_BALL 4
#define ACTION_EXTRABALL 8
#define ACTION_SCORE 16

#define INIT_EVENT 9
#define HIGH_EVENT 10
#define OVER_EVENT 11

#define ERR_SD_CARD_NOT_READY 1
#define ERR_FILE_READ 2

class Event;

class SdCard {
public:
    void setup();
    uint8_t status() const { return _status; }

    inline uint8_t error() const { return _error; }
    const char* filename(byte id);
    bool hasAction(byte id, byte action) const;
    inline unsigned long highScore() const { return _highScore; }
    unsigned long number(byte id) const;
    void saveHighScore(unsigned long highScore);
private:
    uint8_t _status;
    void loadConfig();
    Event* lookupPort(char ch);
    Event* _events;
    char _filename[];
    byte _error;
    unsigned long _highScore;

    void init(byte id);
    void loadHighScore();
    void loadProgram(Event* event);
};

#endif
