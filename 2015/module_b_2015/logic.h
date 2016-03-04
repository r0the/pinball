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

#ifndef LOGIC_H
#define LOGIC_H

#include <Arduino.h>

#define SCORE_CHANGED      0x01
#define NEW_HIGH_SCORE     0x02
#define NEW_HIGH_SCORE_NOW 0x04
#define BALLS_CHANGED      0x10
#define GAME_OVER          0x20
#define GAME_OVER_NOW      0x40

class Logic {
public:
    Logic();
    void addBall();
    void addScore(unsigned long scoreToAdd);
    void addScoreSlow(unsigned long scoreToAdd);
    void beginLoop();
    void endLoop();
    inline bool hasState(byte state) const { return (_state & state) == state; }
    inline unsigned long highScore() const { return _highScore; }
    void loseBall();
    void newGame(byte balls);
    inline unsigned long score() const { return _score; }
    void setHighScore(unsigned long highScore);
    void setScore(unsigned long score);
private:
    byte _balls;
    unsigned long _highScore;
    byte _oldBalls;
    unsigned long _oldScore;
    unsigned long _score;
    unsigned long _scoreToAdd;
    byte _state;
    inline void setState(byte state, bool set) { if (set) { _state |= state; } else { _state &= ~state; } }
};

#endif

