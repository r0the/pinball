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
 
#include <SD.h>
#include "scroll_text.h"
#include "sure_display.h"

char* strnmove(char* target, const char* source, size_t targetSize) {
    if (targetSize > 0) {
        size_t copySize = strlen(source);
        if (copySize + 1 > targetSize) {
            copySize = targetSize - 1;
        }

        memmove(target, source, copySize);
        target[copySize] = 0;
    }

    return target;
}

ScrollText::ScrollText() :
    _scrollPos(-1),
    _text(new char[MAX_TEXT_SIZE + 1]),
    _textWidth(0)
{
    clear();
    _text[0] = '\0';
}

boolean ScrollText::isFinished() const {
    return _scrollPos > _textWidth;
}

void ScrollText::loop() {
    if (isFinished()) {
        return;
    }

    long now = millis();
    if (now > _nextScrollTime) {
        write(-_scrollPos, _text);
        ++_scrollPos;
        _nextScrollTime = now + 50;
    }
}

void ScrollText::loadText(const char* filename) {
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return;
    }

    byte pos = 0;
    while (file.available() && pos < MAX_TEXT_SIZE) {
        _text[pos] = file.read();
        ++pos;
    }

    _text[pos] = '\0';
    file.close();
    reset();
}

void ScrollText::setText(const char* text) {
    strnmove(_text, text, MAX_TEXT_SIZE + 1);
    reset();
}

void ScrollText::setTextProgmem(const char* text) {
    strcpy_P(_text, text);
    reset();
}

void ScrollText::reset() {
    clear();
    _textWidth = textWidth(_text);
    _scrollPos = -displayWidth();
    _nextScrollTime = millis();
}

