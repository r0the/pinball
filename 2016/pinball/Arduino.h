#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtGlobal>
#include <QFile>
#include <QDebug>
#include <iostream>

// types

typedef quint8 uint8_t;
typedef quint16 uint16_t;
typedef quint32 uint32_t;

// Arduino pin handling

#define A0 1
#define A1 1
#define A2 1
#define A3 1
#define A4 1
#define A5 1

#define INPUT_PULLUP 1
#define OUTPUT 0

#define LOW 0
#define HIGH 1

void pinMode(quint8 pin, quint8 mode);

void digitalWrite(uint8_t pin, uint8_t value);

uint16_t digitalRead(uint8_t pin);

//

#define PROGMEM

#define FILE_READ QIODevice::ReadOnly

uint32_t millis();

// Interrupt handling

void cli();

void sei();

#define ISR(X) void isr()

// registers

extern uint32_t OCR1BH;
extern uint32_t OCR1BL;
extern uint32_t TCCR1A;
extern uint32_t TCCR1B;
extern uint32_t COM1B1;
extern uint32_t TIMSK1;

#define _BV(X) (1 << X)
#define TOIE1 1

#endif // ARDUINO_H
