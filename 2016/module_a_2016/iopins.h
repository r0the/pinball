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
#include "hardware.h"

class IoPins {
public:
    IoPins();
    void setup();
    void loop();
    bool hasEvent(uint8_t pin) const;
    void setHigh(uint8_t pin);
    void setLow(uint8_t pin);
    void setOutput(uint8_t pin);
private:
    IoPins(const IoPins&);
    IoPins& operator=(const IoPins&);
    uint8_t lookupPin(uint8_t pin) const;

    uint32_t _config;
    uint32_t _inputLock[IO_PIN_COUNT];
    uint8_t _inputStates[IO_PIN_COUNT];
};

#endif

