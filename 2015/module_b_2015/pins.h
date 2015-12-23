/*
 * Copyright (C) 2015 by Stefan Rothe
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
 
#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

#define INPUT_DELAY 100

class Pins {
public:
    Pins(byte count, const byte* pins);

    void loop();
    
    inline int count() const {
        return _count;
    }

    inline boolean hasEvent(byte pin) const {
        return _inputStates[pin] >= INPUT_DELAY;
    }

    void setLow(int pin);
    void setHigh(int pin);
private:
    byte* _config;
    byte _count;
    unsigned long* _inputLock;
    byte* _inputStates;
    const byte* _pins;
};

#endif
