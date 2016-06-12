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
#include <SD.h>

// configuration
#define WGM 1
#define CS 1
#define BUFFER_SIZE 32

#define AUDIO_PLAYING 0x80
#define AUDIO_BUFFER_SELECT 0x01
/*
 * audio state
 * 
 * | Bit | Description                |
 * | --- | -------------------------- |
 * |   7 | playback is running        |
 * |   6 |
 * |   5 |
 * |   4 |
 * |   3 |
 * |   2 | buffer 1 ready to play     |
 * |   1 | buffer 0 ready to play     |
 * |   0 | playback buffer select     |
 */
static volatile uint8_t audioState = 0;
static uint8_t buffer[2][BUFFER_SIZE];
static volatile uint8_t counter = 0;
static volatile uint8_t playbackPos = 0;
static File file;

static void loadBuffer(uint8_t loadBuffer) {
    uint16_t size = file.read(reinterpret_cast<uint8_t*>(buffer[loadBuffer]), BUFFER_SIZE);
    if (size < BUFFER_SIZE) {
        // TODO: first play last samples
        file.close();
        audioState &= ~AUDIO_PLAYING;
    }
    else {
        // set ready to play flag
        audioState |= 0x02 << loadBuffer;
    }
}

static void checkBuffer() {
    cli();
    if (!(audioState & 0x02)) {
        loadBuffer(0);
    }

    if (!(audioState & 0x04)) {
        loadBuffer(1);
    }

    sei();
}

ISR(TIMER1_OVF_vect) {
    if (!(audioState & AUDIO_PLAYING)) {
        return;
    }

    if (counter < 1) {
//        checkBuffer();
        ++counter;
    }
    else {
        counter = 0;
        // get the current playback buffer (bit 0 of audio state)
        uint8_t playbackBuffer = audioState & 0x01;
        // check if the current playback buffer is ready to play
        if (audioState & (0x02 << playbackBuffer)) {
            // read the next sample from the buffer
            OCR1BL = buffer[playbackBuffer][playbackPos];
            ++playbackPos;
            // check if buffer switch is needed
            if (playbackPos >= BUFFER_SIZE) {
                playbackPos = 0;
                // clear ready to play flag
                audioState &= ~(0x02 << playbackBuffer);
                // switch playback buffer
                if (playbackBuffer == 0) {
                    audioState |= 0x01;
                }
                else {
                    audioState &= ~0x01;
                }
            }
        }
        else {
            OCR1BL = 0;
        }
    }
}

Audio::Audio() {
}

Audio::~Audio() {
    stop();
}

void Audio::begin() {
    cli();
    pinMode(PIN_PWM, OUTPUT);

    /* 
     * TCCR1B - Timer/Counter1 Control Register B
     *
     * | Bit | Name  | Value     | Meaning                  |
     * | --- | ----- | --------- | ------------------------ |
     * |   7 | ICNC1 | 0         | irrelevant               |
     * |   6 | ICES1 | 0         | irrelevant               |
     * |   5 | -     | 0         | -                        |
     * |   4 | WGM13 | WGM bit 3 | Waveform generation mode |
     * |   3 | WGM12 | WGM bit 2 | Waveform generation mode |
     * |   2 | CS12  | CS bit 2  | Clock select             |
     * |   1 | CS11  | CS bit 1  | Clock select             |
     * |   0 | CS10  | CS bit 0  | Clock select             |
     */
    TCCR1B = ((WGM & 0x0C) << 1) | (CS & 0x07);

    /*
     * TCCR1A - Timer/Counter1 Control Register A
     *
     * | Bit | Name   | Value     | Meaning                       |
     * | --- | ------ | --------- | ----------------------------- |
     * |   7 | COM1A1 | 0         | OC1A (Pin 9) disconnected     |
     * |   6 | COM1A0 | 0         | OC1A (Pin 9) disconnected     |
     * |   5 | COM1B1 | 1         | OC1B (Pin 10) clear on match  |
     * |   4 | COM1B0 | 0         | OC1B (Pin 10) clear on match  |
     * |   3 | -      | 0         | -                             |
     * |   2 | -      | 0         | -                             |
     * |   1 | WGM11  | WGM bit 1 | Waveform generation mode      |
     * |   0 | WGM10  | WGM bit 0 | Waveform generation mode      |
     */
    TCCR1A = B00100000 | (WGM & 0x03);

    /* 
     *  
     * TIMSK1 - Timer/Counter1 Interrupt Mask Register
     * 
     * | Bits  | Name           | Value | Meaning                       |
     * | ----- | -------------- | ----- | ----------------------------- |
     * | 7/6   | -              | 00    | -                             |
     * | 5     | ICIE1          | 1     | enable timer 1 interrupt      |
     * | 4/3   | -              | 00    | -                             |
     * | 2/1   | OCIE1B/OCIE1A  | 00    | irrelevant                    |
     * | 0     | TOIE1          | 1     | enable interrupt on top       |
     * 
     */
    TIMSK1 = 0;
    //TIMSK1 = B00100001;

    // ICR1H/ICR1L - Input Capture Register 1
    //
    // This register is used for defining the counter TOP value
    // 
//    ICR1H = 0x0000;
//    ICR1L = 0x007F;
    OCR1BH = 0;
    OCR1BL = 250;
    sei();
}

bool Audio::play(const char* filename) {
    stop();
    char tmp[4];
    file = SD.open(filename, FILE_READ);
    // Read RIFF-WAVE header
    if (file.read(tmp, 4) < 4) {
        return false;
    }

    if (tmp[0] != 'R' || tmp[1] != 'I' || tmp[2] != 'F' || tmp[3] != 'F') {
        return false;
    }

    uint32_t size = 0;
    if (file.read(reinterpret_cast<uint8_t*>(&size), 4) < 4) {
        file.close();
        return false;
    }

    if (file.read(tmp, 4) < 4) {
        return false;
    }

    if (tmp[0] != 'W' || tmp[1] != 'A' || tmp[2] != 'V' || tmp[3] != 'E') {
        return false;
    }

    // Read format chunk
    if (file.read(tmp, 4) < 4) {
        return false;
    }

    if (tmp[0] != 'f' || tmp[1] != 'm' || tmp[2] != 't' || tmp[3] != ' ') {
        return false;
    }

    uint32_t fmtSize = 0;
    if (file.read(reinterpret_cast<uint8_t*>(&fmtSize), 4) < 4) {
        file.close();
        return false;
    }

    // skip format chunk
    for (int i = 0; i < fmtSize; ++i) {
        file.read();
    }

    // Read data chunk header
    if (file.read(tmp, 4) < 4) {
        return false;
    }

    if (tmp[0] != 'd' || tmp[1] != 'a' || tmp[2] != 't' || tmp[3] != 'a') {
        return false;
    }

    // now positioned at the first sample
    audioState = AUDIO_PLAYING;
    loadBuffer(0);
    loadBuffer(1);
    playbackPos = 0;
    TIMSK1 = B00100001;
    return true;
}

bool Audio::playing() const {
    return audioState & AUDIO_PLAYING;
}

void Audio::stop() {
    if (audioState & AUDIO_PLAYING) {
        file.close();
        audioState &= ~AUDIO_PLAYING;
    }
}

void Audio::loop() {
    checkBuffer();
}

