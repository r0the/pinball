#include "virtual_display.h"
#include "scroll_text.h"
#include "sure_display.h"
#include "logic.h"
#include "_TMRpcm.h"
#include "pins.h"
#include "sdcard.h"

// --------------------------------------------------------------------------------------------------
// Customization
// --------------------------------------------------------------------------------------------------
//#define CHRISTOPHE
//#define NOELL
//#define STEFAN


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

#define INIT_DELAY 6000

// Parse modes
#define PARSE_COMMAND 1
#define PARSE_NUMBER 2

// --------------------------------------------------------------------------------------------------
// Messages.

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
    byte _demoMode;
    unsigned long _initTime;
    Logic _logic;
    Pins _pins;
    VirtualDisplay _scoreDisplay;
    ScrollText _scrollText;
    SdCard _sdCard;
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
                    _logic.setScore(i + 1);
                }
            }
        }
    }

    void game() {
        _logic.beginLoop();

        // Process inputs
        _pins.loop();
        for (int i = 0; i < _pins.count(); ++i) {
            if (_pins.hasEvent(i)) {
                event(i);
            }
        }

        // Process delayed score
        _logic.endLoop();
        if (_logic.hasState(NEW_HIGH_SCORE_NOW)) {
            event(HIGH_EVENT);
        }

        unsigned long now = millis();
        if (_initTime != 0 && _initTime < now) {
            event(INIT_EVENT);
            _initTime = 0;      
        }

        if (_logic.hasState(GAME_OVER_NOW)) {
            event(OVER_EVENT);
            _initTime = now + INIT_DELAY;

            if (_logic.hasState(NEW_HIGH_SCORE)) {
                _sdCard.saveHighScore(_logic.highScore());
            }
        }
    }

    void event(byte id) {
        customEvent(id + 1);
        if (_sdCard.hasAction(id, ACTION_SCORE)) {
            _logic.addScore(_sdCard.number(id));
        }

        if (_sdCard.hasAction(id, ACTION_LOSE_BALL)) {
            _logic.loseBall();
        }

        if (_sdCard.hasAction(id, ACTION_EXTRABALL)) {
            _logic.addBall();
        }

        if (_sdCard.hasAction(id, ACTION_TEXT)) {
            _scrollText.loadText(_sdCard.filename(id));
        }

        _sdCard.play(id);
    }

public:
    PinballProgram(int pinCount, const byte* pins) :
        _demoMode(0),
        _initTime(0),
        _pins(pinCount, pins),
        _scoreDisplay(),
        _scrollText(),
        _sdCard(SD_CARD_CHIP_SELECT_PIN, SPEAKER_PIN),
        _showText(false)
    {
        _scoreDisplay.addDisplay(new SureDisplay(DISPLAY_CHIP_SELECT_PIN_B, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN));
        _scoreDisplay.clear();
        _scoreDisplay.showNumber(_logic.score(), 5);
        _scrollText.addDisplay(new SureDisplay(DISPLAY_CHIP_SELECT_PIN_A, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN));
        _scrollText.addDisplay(new SureDisplay(DISPLAY_CHIP_SELECT_PIN_B, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN));
        if (_sdCard.error() > 0) {
            _scoreDisplay.showNumber(_sdCard.error(), 5);
        }
        else {
            _scoreDisplay.showNumber(_logic.score(), 5);
        }

        _logic.setHighScore(_sdCard.highScore());
    }

    void init() {
        event(INIT_EVENT);
        customInit();
    }

    void loop() {
        unsigned long oldScore = _logic.score();
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
                _scoreDisplay.showNumber(_logic.score(), 5);
            }
        }
        else {        
            // Update score on display if it has changed
            if (_logic.score() != oldScore) {
                _scoreDisplay.showNumber(_logic.score(), 5);
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

