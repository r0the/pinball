/*
 * Copyright (C) 2014 - 2015 by Stefan Rothe
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
 
 #include "virtual_display.h"

#define MAX_DISPLAY_COUNT 4

#define DISPLAY_WIDTH 32

PROGMEM const byte FONT[] PROGMEM = {
    B00000000, B00000000, B00000000, B00000000, B00000000, 3, // SPACE
    B01011111, B00000000, B00000000, B00000000, B00000000, 1, // !
    B00000011, B00000000, B00000011, B00000000, B00000000, 3, // "
    B00010100, B01111111, B00010100, B01111111, B00010100, 5, // #
    B00100100, B00101010, B01101011, B00101010, B00010010, 5, // $
    B00100011, B00010011, B00001000, B01100100, B01100010, 5, // %
    B00000000, B00000000, B00000000, B00000000, B00000000, 0, // & TODO
    B00000011, B00000000, B00000000, B00000000, B00000000, 1, // '

    B00011100, B00100010, B01000001, B00000000, B00000000, 3, // (
    B01000001, B00100010, B00011100, B00000000, B00000000, 3, // )
    B00101010, B00011100, B00111110, B00011100, B00101010, 5, // * IMPROVE
    B00001000, B00001000, B00111110, B00001000, B00001000, 5, // +
    B10000000, B01000000, B00000000, B00000000, B00000000, 2, // ,
    B00001000, B00001000, B00001000, B00001000, B00001000, 5, // -
    B01000000, B01000000, B00000000, B00000000, B00000000, 1, // .
    B00100000, B00010000, B00001000, B00000100, B00000010, 5, // /

    B00111110, B01010001, B01001001, B01000101, B00111110, 5, // 0
    B01000100, B01000010, B01111111, B01000000, B01000000, 5, // 1
    B01100010, B01010001, B01001001, B01001001, B01000110, 5, // 2
    B00100010, B01000001, B01001001, B01001001, B00110110, 5, // 3
    B00011000, B00010100, B00010010, B01111111, B00010000, 5, // 4
    B00100111, B01000101, B01000101, B01000101, B00111001, 5, // 5
    B00111100, B01001010, B01001001, B01001001, B00110000, 5, // 6
    B00000001, B01110001, B00001001, B00000101, B00000011, 5, // 7
    
    B00110110, B01001001, B01001001, B01001001, B00110110, 5, // 8
    B00000110, B01001001, B01001001, B00101001, B00011110, 5, // 9
    B01001000, B00000000, B00000000, B00000000, B00000000, 1, // :
    B10000000, B01001000, B00000000, B00000000, B00000000, 2, // ;
    B00001000, B00010100, B00100010, B01000001, B00000000, 4, // <
    B00010100, B00010100, B00010100, B00010100, B00010100, 5, // =
    B01000001, B00100010, B00010100, B00001000, B00000000, 4, // >
    B00000010, B00000001, B01010001, B00001001, B00000110, 5, // ?
    
    B00111110, B01000001, B01011101, B01010101, B01011110, 5, // @
    B01111110, B00001001, B00001001, B00001001, B01111110, 5, // A
    B01111111, B01001001, B01001001, B01001001, B00110110, 5, // B
    B00111110, B01000001, B01000001, B01000001, B00100010, 5, // C
    B01111111, B01000001, B01000001, B00100010, B00011100, 5, // D
    B01111111, B01001001, B01001001, B01001001, B01000001, 5, // E
    B01111111, B00001001, B00001001, B00001001, B00000001, 5, // F
    B00111110, B01000001, B01001001, B01001001, B00111010, 5, // G
    
    B01111111, B00001000, B00001000, B00001000, B01111111, 5, // H
    B01000001, B01111111, B01000001, B00000000, B00000000, 3, // I
    B00110000, B01000000, B01000001, B00111111, B00000001, 5, // J
    B01111111, B00001000, B00010100, B00100010, B01000001, 5, // K
    B01111111, B01000000, B01000000, B01000000, B01000000, 5, // L
    B01111111, B00000010, B00001100, B00000010, B01111111, 5, // M
    B01111111, B00000100, B00001000, B00010000, B01111111, 5, // N
    B00111110, B01000001, B01000001, B01000001, B00111110, 5, // O
    
    B01111111, B00001001, B00001001, B00001001, B00000110, 5, // P
    B00111110, B01000001, B01100001, B01000001, B10111110, 5, // Q
    B01111111, B00001001, B00011001, B00101001, B01000110, 5, // R
    B00100110, B01001001, B01001001, B01001001, B00110010, 5, // S
    B00000001, B00000001, B01111111, B00000001, B00000001, 5, // T
    B00111111, B01000000, B01000000, B01000000, B00111111, 5, // U
    B00011111, B00100000, B01000000, B00100000, B00011111, 5, // V
    B00111111, B01000000, B00111000, B01000000, B00111111, 5, // W
    
    B01100011, B00010100, B00001000, B00010100, B01100011, 5, // X
    B00000111, B00001000, B01110000, B00001000, B00000111, 5, // Y
    B01100001, B01010001, B01001001, B01000101, B01000011, 5, // Z
    B01111111, B01000001, B01000001, B00000000, B00000000, 3, // [
    B00000010, B00000100, B00001000, B00010000, B00100000, 5, // backslash
    B01000001, B01000001, B01111111, B00000000, B00000000, 3, // ]
    B00000100, B00000010, B00000001, B00000010, B00000100, 5, // ^
    B01000000, B01000000, B01000000, B01000000, B01000000, 5, // _
    B00000001, B00000010, B00000000, B00000000, B00000000, 2, // back quote
    B00100000, B01010100, B01010100, B01010100, B01111000, 5, // a
    B01111111, B01000100, B01000100, B01000100, B00111000, 5, // b
    B00111000, B01000100, B01000100, B01000100, B01001000, 5, // c
    B00111000, B01000100, B01000100, B01000100, B01111111, 5, // d
    B00111000, B01010100, B01010100, B01010100, B01011000, 5, // e
    B00000100, B01111110, B00000101, B00000001, B00000000, 4, // f
    B10011000, B10100100, B10100100, B10100100, B01111000, 5, // g
    B01111111, B00000100, B00000100, B00000100, B01111000, 5, // h
    B01000100, B01111101, B01000000, B00000000, B00000000, 3, // i
    B01000000, B10000000, B10000100, B01111101, B00000000, 4, // j
    B01111111, B00010000, B00101000, B01000100, B00000000, 4, // k
    B01000001, B01111111, B01000000, B00000000, B00000000, 3, // l
    B01111100, B00000100, B01111100, B00000100, B01111000, 5, // m
    B01111100, B00000100, B00000100, B00000100, B01111000, 5, // n
    B00111000, B01000100, B01000100, B01000100, B00111000, 5, // o
    B11111100, B00100100, B00100100, B00100100, B00011000, 5, // p
    B00011000, B00100100, B00100100, B00100100, B11111100, 5, // q
    B00000100, B01111000, B00000100, B00000100, B00001000, 5, // r
    B01001000, B01010100, B01010100, B01010100, B00100100, 5, // s
    B00000100, B00111111, B01000100, B01000100, B00000000, 4, // t
    B00111100, B01000000, B01000000, B01000000, B01111100, 5, // u
    B00001100, B00110000, B01000000, B00110000, B00001100, 5, // v
    B00111100, B01000000, B00111100, B01000000, B00111100, 5, // w
    B01000100, B00101000, B00010000, B00101000, B01000100, 5, // x
    B00011100, B10100000, B10100000, B10100000, B01111100, 5, // y
    B01000100, B01100100, B01010100, B01001100, B01000100, 5, // z
    B00001000, B00110110, B01000001, B01000001, B00000000, 4, // {
    B01111111, B00000000, B00000000, B00000000, B00000000, 1, // |
    B01000001, B01000001, B00110110, B00001000, B00000000, 4, // }
    B00000010, B00000001, B00000010, B00000001, B00000000, 4, // ~
};

#define CHAR_BYTES 6
#define MIN_CHAR 32
#define MAX_CHAR 126

byte readFont(char ch, int offset) {
    if (MIN_CHAR <= ch && ch <= MAX_CHAR) {
        return pgm_read_byte_near(FONT + (ch - MIN_CHAR) * CHAR_BYTES + offset);
    }
    else {
        return 0;
    }
}

VirtualDisplay::VirtualDisplay() {
    _displayCount = 0;
    _displays = new const SureDisplay*[MAX_DISPLAY_COUNT];
    clear();
}

void VirtualDisplay::addDisplay(const SureDisplay* display) {
    if (_displayCount < MAX_DISPLAY_COUNT) {
        _displays[_displayCount] = display;
        ++_displayCount;
    }
}

void VirtualDisplay::clear() {
    for (int pos = 0; pos < _displayCount * DISPLAY_WIDTH; ++pos) {
        setColumn(pos, 0);
    }
}

int VirtualDisplay::displayWidth() const {
    return _displayCount * DISPLAY_WIDTH;
}

void VirtualDisplay::setColumn(int pos, byte data) {
    if (0 <= pos && pos < DISPLAY_WIDTH * _displayCount) {
        _displays[pos / DISPLAY_WIDTH]->setColumn(pos % DISPLAY_WIDTH, data);
    }
}

void VirtualDisplay::showNumber(unsigned long number, byte width) {
    int pos = writeNumber(DISPLAY_WIDTH * _displayCount - 1, number, width);
    for (int i = 0; i < pos; ++i) {
        setColumn(i, 0);
    }
}

int VirtualDisplay::textWidth(const char* text) const {
    int result = 0;
	int i = 0;
    while (text[i] != '\0') {
        result += readFont(text[i], CHAR_BYTES - 1) + 1;
        ++i;
    }

    return result - 1;
}

int VirtualDisplay::write(int pos, const char* text) {
    int i = 0;
    while (text[i] != '\0') {
        pos = writeChar(pos, text[i], LR);
        ++i;
    }

    return pos;
}

int VirtualDisplay::writeChar(int pos, char ch, boolean direction) {
    int charWidth = readFont(ch, CHAR_BYTES - 1);
    if (direction == LR) {
        for (int i = 0; i < charWidth; ++i) {
            setColumn(pos, readFont(ch, i));
            ++pos;
        }

        setColumn(pos, 0);
        return pos + 1;
    }
    else {
        for (int i = charWidth - 1; i >= 0; --i) {
            setColumn(pos, readFont(ch, i));
            --pos;
        }

        setColumn(pos, 0);
        return pos - 1;
    }
}

int VirtualDisplay::writeNumber(int pos, unsigned long number, byte width) {
    int w = 0;
	while (number > 0) {
        pos = writeChar(pos, '0' + number % 10, RL);
        ++w;
        number = number / 10;
    }

    while (w < width) {
        pos = writeChar(pos, '0', RL);
        ++w;
    }

    return pos;
}

