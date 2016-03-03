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

#include "logic.h"

// Score points to add in each step when counting slow
#define SCORE_STEP 10

Logic::Logic() {
    newGame(3);
}

void Logic::addBall() {
    if (_balls > 0) {
        ++_balls;
    }
}

void Logic::addScore(unsigned long scoreToAdd) {
    if (_balls > 0) {
        _score += scoreToAdd;
    }
}

void Logic::addScoreSlow(unsigned long scoreToAdd) {
    if (_balls > 0) {
        _scoreToAdd += scoreToAdd;
    }
}

void Logic::beginLoop() {
    _oldBalls = _balls;
    _oldScore = _score;    
}

void Logic::endLoop() {
    // Process delayed score
    if (_scoreToAdd >= SCORE_STEP) {
        _score += SCORE_STEP;
        _scoreToAdd -= SCORE_STEP;
    }
    else if (_scoreToAdd > 0) {
        _score += _scoreToAdd;
        _scoreToAdd = 0;
    }

    setState(SCORE_CHANGED, _score != _oldScore);
    setState(BALLS_CHANGED, _balls != _oldBalls);

    if (_highScore < _score) {
        if (_state & NEW_HIGH_SCORE) {
            _state &= ~NEW_HIGH_SCORE_NOW;
        }
        else {
            _state |= NEW_HIGH_SCORE | NEW_HIGH_SCORE_NOW;
        }
    }

    if (_state & BALLS_CHANGED && _balls == 0) {
        if (_state & GAME_OVER) {
            _state &= ~GAME_OVER_NOW;
        } else {
            _state |= GAME_OVER | GAME_OVER_NOW;            
        }
    }
}

void Logic::loseBall() {
    if (_balls > 0) {
        --_balls;
    }
}

void Logic::newGame(byte balls) {
    _balls = 3;
    _state = 0;
    _score = 0;
    _scoreToAdd = 0;
}

void Logic::setHighScore(unsigned long highScore) {
    _highScore = highScore;
}

void Logic::setScore(unsigned long score) {
    _score = score;
}

