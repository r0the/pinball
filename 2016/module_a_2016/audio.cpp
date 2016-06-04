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

#include "audio.h"
#include "hardware.h"

void Audio::setup() {
    _filename = new char[9];
    _filename[4] = '.';
    _filename[5] = 'w';
    _filename[6] = 'a';
    _filename[7] = 'v';
    _filename[8] = '\0';
    _tmrpcm.speakerPin = PIN_PWM;
}

void Audio::play(const char* name) {
//        strcpy_P(_filename, (char*) pgm_read_word(&(eventName[id])));
    _filename[0] = name[0];
    _filename[1] = name[1];
    _filename[2] = name[2];
    _filename[3] = name[3];
    Serial.print("Playing ");
    Serial.println(_filename);
    if (_tmrpcm.isPlaying()) {
        _tmrpcm.stopPlayback();
    }
    
    _tmrpcm.play(_filename);
}

