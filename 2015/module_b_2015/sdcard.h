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

#include "_TMRpcm.h"

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
    SdCard(byte sdChipSelectPin, byte speakerPin);
    inline byte error() const { return _error; }
    const char* filename(byte id);
    bool hasAction(byte id, byte action) const;
    inline unsigned long highScore() const { return _highScore; }
    unsigned long number(byte id) const;
    void play(byte id);
    void saveHighScore(unsigned long highScore);
private:
    TMRpcm _audio;
    Event* _events;
    char _filename[];
    byte _error;
    unsigned long _highScore;
    void init(byte id);
    void loadHighScore();
    void loadProgram(Event* event);
};

#endif
