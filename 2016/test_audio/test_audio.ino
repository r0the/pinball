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

Audio audio;

void setup() {
    SD.begin(1);
    
    audio.begin();
    audio.play("test.wav");
}

void loop() {
    audio.loop();
}

