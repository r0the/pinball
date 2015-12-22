#include <SD.h>
#include <SPI.h>
#include "virtual_display.h"
#include "scroll_text.h"
#include "sure_display.h"
#include "_TMRpcm.h"

// --------------------------------------------------------------------------------------------------
// Pins
// --------------------------------------------------------------------------------------------------

#define DISPLAY_CLOCK_PIN 4
#define DISPLAY_CHIP_SELECT_PIN_A 5
#define DISPLAY_CHIP_SELECT_PIN_B 6
#define DISPLAY_DATA_PIN 8
#define SPEAKER_PIN 9
#define SD_CARD_CHIP_SELECT_PIN 10

const byte INPUT_PINS[] = { A5, A4, A3, A2, A1, A0, 1, 2, 3 };
#define INPUT_COUNT 9

#define INPUT_DELAY 50

// --------------------------------------------------------------------------------------------------
// Other constants
// --------------------------------------------------------------------------------------------------

// Events
#define INIT_EVENT 9
#define HIGH_EVENT 10
#define OVER_EVENT 11

// Event names
const char* EVENT_NAME[] = { 
    "in01", "in02", "in03", "in04", "in05", "in06", "in07", "in08", "in09",
    "init", "high", "over"
};

// Score points to add in each step when counting slow
#define SCORE_STEP 10

// Parse modes
#define PARSE_COMMAND 1
#define PARSE_NUMBER 2

// --------------------------------------------------------------------------------------------------
// Messages
// --------------------------------------------------------------------------------------------------

const char SD_CARD_NOT_READY[] PROGMEM = "ERROR: SD card not ready.";
const char DEMO_TEXT[] PROGMEM = "FF Technisches Konstruieren";

// --------------------------------------------------------------------------------------------------
// Input
// --------------------------------------------------------------------------------------------------

class Input {
    byte _inputCount;
    unsigned long* _inputLocks;
    const byte* _inputPins;
    byte* _inputPinStates;

public:
    Input(byte inputCount, const byte* inputPins) :
        _inputCount(inputCount),
        _inputLocks(new unsigned long[inputCount]),
        _inputPins(inputPins),
        _inputPinStates(new byte[inputCount])
    {
        for (int i = 0; i < _inputCount; ++i) {
            _inputLocks[i] = 0;
            _inputPinStates[i] = 0;
            pinMode(_inputPins[i], INPUT_PULLUP);
        }
    }

    void loop() {
        unsigned long now = millis();
        for (int i = 0; i < _inputCount; ++i) {
            if (_inputLocks[i] < now) {
                int state = digitalRead(_inputPins[i]);
                if (state == HIGH) {
                    _inputPinStates[i] = 0;
                }
                else {
                    if (_inputPinStates[i] < INPUT_DELAY) {
                        ++_inputPinStates[i];
                    }
                    else {
                        _inputPinStates[i] = 0;
                    }
                }
            }
        }
    }
    
    int count() const {
        return _inputCount;
    }

    boolean hasEvent(byte i) const {
        return _inputPinStates[i] == INPUT_DELAY;
    }
};

// --------------------------------------------------------------------------------------------------
// Program
// --------------------------------------------------------------------------------------------------

#define MAX_COMMAND_SIZE 50
#define MAX_FILENAME_SIZE 12
#define MAX_TEXT_SIZE 40

class PinballProgram {
    TMRpcm _audio;
    byte _balls;
    byte _demoMode;
    char* _filename;
    unsigned long _highScore;
    Input _input;
    boolean _newHighScore;
    unsigned long _score;
    unsigned long _scoreToAdd;
    VirtualDisplay _scoreDisplay;
    ScrollText _scrollText;
    boolean _showText;

    void saveHighScore() {
        File file = SD.open("hiscore.dat", FILE_WRITE);
        unsigned long number = _highScore;
        while (number > 0) {
            file.write('0' + (number % 10));
            number = number / 10;
        }

        file.close();
    }

    void loadHighScore() {
        _highScore = 0;
        File file = SD.open("hiscore.dat", FILE_READ);
        while (file.available()) {
            char ch = file.read();
            if ('0' <= ch && ch <= '9') {
                _highScore = _highScore * 10 + (ch - '0');
            }
        }

        file.close();
    }

    void demo() {
        unsigned long time = millis();
        if (_demoMode == 1 && time > 1000) {
            _scrollText.setTextProgmem(DEMO_TEXT);
            _showText = true;
            _demoMode = 2;
            randomSeed(analogRead(0));
        }

        if (_demoMode == 2) {
            for (int i = 0; i < _input.count(); ++i) {
                if (_input.hasEvent(i)) {
                    _score = i + 1;
                }
            }
        }

        if (_demoMode == 3) {
            long r = random(500000);
            if (r == 0) {
                _score += 1000;
            }
            else if (r < 10) {
                _score += 100;
            }
            else if (r < 20) {
                _score += 10;
            }
            else if (r < 50) {
                _score += 5;
            }
        }
    }

    void game() {
        byte oldBalls = _balls;

        // Process inputs
        _input.loop();
        for (int i = 0; i < _input.count(); ++i) {
            if (_input.hasEvent(i)) {
                event(i);
            }
        }

        // Process delayed score
        if (_scoreToAdd >= SCORE_STEP) {
            _score += SCORE_STEP;
            _scoreToAdd -= SCORE_STEP;
        }
        else if (_scoreToAdd > 0) {
            _score += _scoreToAdd;
            _scoreToAdd = 0;
        }

        if (!_newHighScore && _highScore < _score) {
            event(HIGH_EVENT);
            _newHighScore = true;
        }

        if (_balls != oldBalls && _balls == 0) {
            event(OVER_EVENT);
            if (_newHighScore) {
                saveHighScore();
                _newHighScore = false;
            }
        }
    }

    void executeCommand(char command, unsigned long number) {
        switch (command) {
        case '+':
            if (_balls > 0) {
                _score += number;
            }

            break;
        case '*':
            if (_balls > 0) {
                _scoreToAdd += number;
            }

            break;
        case 'n':
            _score = 0;
            _scoreToAdd = 0;
            _balls = number;
            break;
        case 'b':
            if (_balls > 0) {
                --_balls;
            }

            break;
        case 'x':
            if (_balls > 0) {
                ++_balls;
            }

            break;
        }
    }

    void executeProgram(const char* filename) {
        File file = SD.open(filename, FILE_READ);
        if (!file) {
            // ignore
            return;
        }

        byte mode = PARSE_COMMAND;
        char command = '\0';
        char ch = '\0';
        unsigned long number = 0;
        while (file.available()) {
            ch = file.read();
            switch (ch) {
            case '+':
            case '*':
            case 'n':
                if (mode == PARSE_COMMAND) {
                    command = ch;
                    mode = PARSE_NUMBER;
                }
                else {
                    executeCommand(command, number);
                    mode = PARSE_COMMAND;
                }

                break;
            case 'b':
            case 'x':
                executeCommand(command, number);
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (mode == PARSE_NUMBER) {
                    number = number * 10 + (ch - '0');
                }
                break;

            default:
                if (mode == PARSE_NUMBER) {
                    executeCommand(command, number);
                }

                mode = PARSE_COMMAND;
                break;
            }
        }

        if (mode == PARSE_NUMBER) {
            executeCommand(command, number);
        }

        file.close();
    }

    void event(byte id) {
        for (byte i = 0; i < 4; ++i) {
            _filename[i] = EVENT_NAME[id][i];
        }

        _filename[4] = '.';
        _filename[5] = 'w';
        _filename[6] = 'a';
        _filename[7] = 'v';
        _filename[8] = '\0';
        if (SD.exists(_filename)) {
            if (_audio.isPlaying()) {
                _audio.stopPlayback();
            }
    
            _audio.play(_filename);
        }

        _filename[5] = 't';
        _filename[6] = 'x';
        _filename[7] = 't';
        if (SD.exists(_filename)) {
            _scrollText.loadText(_filename);
            _showText = true;
        }

        _filename[5] = 'p';
        _filename[6] = 'r';
        _filename[7] = 'g';
        if (SD.exists(_filename)) {
            executeProgram(_filename);
        }
    }

public:
    PinballProgram(int inputCount, const byte* inputPins) :
        _audio(),
        _balls(3),
        _demoMode(0),
        _filename(new char[9]),
        _input(inputCount, inputPins),
        _score(0),
        _scoreDisplay(),
        _scoreToAdd(0),
        _scrollText(),
        _showText(false)
    {
        _scoreDisplay.addDisplay(new SureDisplay(DISPLAY_CHIP_SELECT_PIN_B, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN));
        _scoreDisplay.clear();
        _scoreDisplay.showNumber(_score, 5);
        _scrollText.addDisplay(new SureDisplay(DISPLAY_CHIP_SELECT_PIN_A, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN));
        _scrollText.addDisplay(new SureDisplay(DISPLAY_CHIP_SELECT_PIN_B, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN));
        _audio.speakerPin = SPEAKER_PIN;
        loadHighScore();
        if (SD.begin(SD_CARD_CHIP_SELECT_PIN)) {
            _scoreDisplay.showNumber(_score, 5);
        }
        else {
            _demoMode = 1;
        }
    }

    void init() {
        event(INIT_EVENT);
        
    }

    void loop() {
        unsigned long oldScore = _score;
        _input.loop();
        if (_demoMode != 0) {
            demo();
        }
        else {
            game();
        }

        if (_showText) {
            _scrollText.loop();
            if (_scrollText.isFinished()) {
                _showText = false;
                _scoreDisplay.showNumber(_score, 5);
            }
        }
        else {        
            // Update score on display if it has changed
            if (_score != oldScore) {
                _scoreDisplay.showNumber(_score, 5);
            }
        }
    }
};

// --------------------------------------------------------------------------------------------------
// Main
// --------------------------------------------------------------------------------------------------

PinballProgram* program;

void setup() {
    program = new PinballProgram(INPUT_COUNT, INPUT_PINS);
    program->init();
}

void loop() {
    program->loop();
}

