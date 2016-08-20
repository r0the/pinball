/*
 * PCM audio playback on Pin 10 of an Arduino Uno
 * 
 * Pin 10 maps to PB2 / SS / OC1B / PCINT2 pin of the ATmega328
 *
 * 
 */

#include <Arduino.h>
#include <SD.h>
#include "audio.h"
#include "display.h"


Audio audio;
Display display;


void setup() {
    SD.begin(1);
    pinMode(9, OUTPUT);
    audio.begin();
//    display.setup();
//    display.showNumber(12345);
    audio.play("test.wav");
}

void loop() {
    
}

