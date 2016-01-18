#include <SD.h>
#include <SPI.h>
#include "virtual_display.h"
#include "scroll_text.h"
#include "sure_display.h"
#include "_TMRpcm.h"
#include "pins.h"

// --------------------------------------------------------------------------------------------------
// Customization
// --------------------------------------------------------------------------------------------------
//#define CHRISTOPHE
//#define NOELL
#define STEFAN


// --------------------------------------------------------------------------------------------------
// Pins
// --------------------------------------------------------------------------------------------------

#define DISPLAY_CLOCK_PIN 4
#define DISPLAY_CHIP_SELECT_PIN_A 5
#define DISPLAY_CHIP_SELECT_PIN_B 6
#define DISPLAY_DATA_PIN 8
#define SPEAKER_PIN 9
#define SD_CARD_CHIP_SELECT_PIN 10

const byte PINS[] = { A5, A4, A3, A2, A1, A0, 1, 2, 3 };
#define PIN_COUNT 9

// --------------------------------------------------------------------------------------------------
// Other constants
// --------------------------------------------------------------------------------------------------

// Events
#define INIT_EVENT 9
#define HIGH_EVENT 10
#define OVER_EVENT 11

const char event_in01[] PROGMEM = "in01";
const char event_in02[] PROGMEM = "in02";
const char event_in03[] PROGMEM = "in03";
const char event_in04[] PROGMEM = "in04";
const char event_in05[] PROGMEM = "in05";
const char event_in06[] PROGMEM = "in06";
const char event_in07[] PROGMEM = "in07";
const char event_in08[] PROGMEM = "in08";
const char event_in09[] PROGMEM = "in09";
const char event_init[] PROGMEM = "init";
const char event_high[] PROGMEM = "high";
const char event_over[] PROGMEM = "over";

// Event names
const char* const eventName[] PROGMEM = {
  event_in01, event_in02, event_in03, event_in04, event_in05, event_in06,
  event_in07, event_in08, event_in09, event_init, event_high, event_over
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
    Pins _pins;
    boolean _newHighScore;
    unsigned long _score;
    unsigned long _scoreToAdd;
    VirtualDisplay _scoreDisplay;
    ScrollText _scrollText;
    boolean _showText;

// --------------------------------------------------------------------------------------------------
// Customization for individual students
// --------------------------------------------------------------------------------------------------

#ifdef CHRISTOPHE
#include "custom_christophe.h"
#elif defined STEFAN
#include "custom_stefan.h"
#elif defined NOELL
#include "custom_noell.h"
#else
#include "custom_empty.h"
#endif

// --------------------------------------------------------------------------------------------------

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
        }

        if (_demoMode == 2) {
            for (int i = 0; i < _pins.count(); ++i) {
                if (_pins.hasEvent(i)) {
                    _score = i + 1;
                }
            }
        }
    }

    void game() {
        byte oldBalls = _balls;

        // Process inputs
        _pins.loop();
        for (int i = 0; i < _pins.count(); ++i) {
            if (_pins.hasEvent(i)) {
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
                    number = 0;
                    mode = PARSE_COMMAND;
                }

                break;
            case 'b':
            case 'x':
                executeCommand(ch, number);
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
                    number = 0;
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
        customEvent(id + 1);
        strcpy_P(_filename, (char*) pgm_read_word(&(eventName[id])));
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
    PinballProgram(int pinCount, const byte* pins) :
        _audio(),
        _balls(3),
        _demoMode(0),
        _filename(new char[9]),
        _pins(pinCount, pins),
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
        customInit();
    }

    void loop() {
        unsigned long oldScore = _score;
        _pins.loop();
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

        customLoop();
    }
};

// --------------------------------------------------------------------------------------------------
// Main
// --------------------------------------------------------------------------------------------------

PinballProgram* program;

void setup() {
    program = new PinballProgram(PIN_COUNT, PINS);
    program->init();
}

void loop() {
    program->loop();
}

