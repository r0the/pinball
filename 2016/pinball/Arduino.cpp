#include "Arduino.h"
#include "SD.h"
#include <QDateTime>

SDCard SD;

uint32_t msec = 0;

void pinMode(quint8 , quint8 ) {
}

void digitalWrite(uint8_t , uint8_t ) {

}

uint32_t millis() {
    ++msec;
    return msec;
}

uint16_t digitalRead(uint8_t ) {
    return HIGH;
}

void cli() {
    // dummy
}

void sei() {
    // dummy
}

uint32_t OCR1BH;
uint32_t OCR1BL;
uint32_t TCCR1A;
uint32_t TCCR1B;
uint32_t COM1B1;
uint32_t TIMSK1;
