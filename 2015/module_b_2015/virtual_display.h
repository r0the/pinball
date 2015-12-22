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
 
#ifndef VIRTUAL_DISPLAY_H
#define VIRTUAL_DISPLAY_H

#include <Arduino.h>
#include "sure_display.h"

#define LR true
#define RL false

class VirtualDisplay {
public:
    VirtualDisplay();
    void addDisplay(const SureDisplay* display);
    void clear();
    int displayWidth() const;
    void setColumn(int pos, byte data);
    void showNumber(unsigned long number, byte width);
    int textWidth(const char* text) const;
    int write(int pos, const char* text);
    int writeChar(int pos, char ch, boolean direction);
    int writeNumber(int pos, unsigned long number, byte width);
private:
    byte _displayCount;
    const SureDisplay** _displays;
};

#endif

