/*
 * Copyright (C) 2015 - 2016 by Stefan Rothe
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
 
#ifndef IO_PINS_H
#define IO_PINS_H

#include <Arduino.h>

class IoPins {
public:
    IoPins();

    void loop();
    boolean hasEvent(uint8_t pin) const;
    void setLow(uint8_t pin);
    void setHigh(uint8_t pin);
private:
    uint8_t* _config;
    uint32_t* _inputLock;
    uint8_t* _inputStates;
};

#endif
