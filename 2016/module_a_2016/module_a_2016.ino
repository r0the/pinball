
#include "display.h"
#include "audio.h"
#include "iopins.h"
#include "logic.h"

#define MODE_CHECK 1
#define MODE_INIT 20;

Display display;
Audio audio;
IoPins pins;
Logic logic;
uint8_t mode;

const char* TEST_WAV = "test.wav";

void setup() {
    audio.begin();
    pins.setup();
    display.setup();
    
//    logic.setup();

    if (!SD.begin(PIN_SD_CHIP_SELECT)) {
        display.show(TEXT_HELLO);
        delay(3000);
        while (!SD.begin(PIN_SD_CHIP_SELECT)) {
            display.show(TEXT_INSRT);
            delay(1000);
            display.show(TEXT_SDCRD);
            delay(1000);
        }
    }

    if (SD.exists(TEST_WAV)) {
        audio.play(TEST_WAV);
        display.show(TEXT_SDRDY);
        delay(1000);
        display.showNumber(88888);
        delay(3000);
    }
    else {
        mode = MODE_INIT;
    }
}

void loopCheck() {
    for (int i = 0; i < IO_PIN_COUNT; ++i) {
        if (pins.hasEvent(i)) {
            display.showPin(i);
        }
    }    
}

void loopReady() {
}

void loop() {
    pins.loop();
    loopCheck();
    if (MODE_CHECK) {
        
    }
    else {
//        loopReady();
    }
}

