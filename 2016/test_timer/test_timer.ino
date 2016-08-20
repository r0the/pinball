/*
 * PCM audio playback on Pin 10 of an Arduino Uno
 * 
 * Pin 10 maps to PB2 / SS / OC1B / PCINT2 pin of the ATmega328
 *
 * 
 */

#include <Arduino.h>

// configuration
#define WGM 1
#define CS 1
#define PIN_PWM 10

const uint8_t values[] = { 10, 50, 100, 200 };
const uint8_t valueCount = sizeof(values) / sizeof(*values);
volatile uint8_t index = 0;

ISR(TIMER1_OVF_vect) {
    OCR1BL = values[index];
    index = (index + 1) % valueCount;
}

void setup() {
    cli();
    pinMode(PIN_PWM, OUTPUT);

    /* 
     * TCCR1B - Timer/Counter1 Control Register B
     *
     * | Bit | Name  | Value     | Meaning                  |
     * | --- | ----- | --------- | ------------------------ |
     * |   7 | ICNC1 | 0         | irrelevant               |
     * |   6 | ICES1 | 0         | irrelevant               |
     * |   5 | -     | 0         | -                        |
     * |   4 | WGM13 | WGM bit 3 | waveform generation mode |
     * |   3 | WGM12 | WGM bit 2 | waveform generation mode |
     * |   2 | CS12  | CS bit 2  | clock select             |
     * |   1 | CS11  | CS bit 1  | clock select             |
     * |   0 | CS10  | CS bit 0  | clock select             |
     */
    TCCR1B = ((WGM & 0x0C) << 1) | (CS & 0x07);

    /*
     * TCCR1A - Timer/Counter1 Control Register A
     *
     * | Bit | Name   | Value     | Meaning                       |
     * | --- | ------ | --------- | ----------------------------- |
     * |   7 | COM1A1 | 0         | OC1A (Pin 9) disconnected     |
     * |   6 | COM1A0 | 0         | OC1A (Pin 9) disconnected     |
     * |   5 | COM1B1 | 1         | OC1B (Pin 10) clear on match  |
     * |   4 | COM1B0 | 0         | OC1B (Pin 10) clear on match  |
     * |   3 | -      | 0         | -                             |
     * |   2 | -      | 0         | -                             |
     * |   1 | WGM11  | WGM bit 1 | waveform generation mode      |
     * |   0 | WGM10  | WGM bit 0 | waveform generation mode      |
     */
    TCCR1A = _BV(COM1B1) | (WGM & 0x03);

    /* 
     * TIMSK1 - Timer/Counter1 Interrupt Mask Register
     * 
     * | Bit  | Name   | Value | Meaning                                 |
     * | ---- | ------ | ----- | --------------------------------------- |
     * |    7 | -      | 0     | -                                       |
     * |    6 | -      | 0     | -                                       |
     * |    5 | ICIE1  | 0     | input capture interrupt enable          |
     * |    4 | -      | 0     | -                                       |
     * |    3 | -      | 0     | -                                       |
     * |    2 | OCIE1B | 0     | output compare B match interrupt enable |
     * |    1 | OCIE1A | 0     | output compare A match interrupt enable |
     * |    0 | TOIE1  | 1     | timer 1 overflow interrupt enable       |
     * 
     */
    TIMSK1 = _BV(TOIE1);

    // ICR1H/ICR1L - Input Capture Register 1
    //
    // This register is used for defining the counter TOP value
    // 
//    ICR1H = 0x0000;
//    ICR1L = 0x007F;
    OCR1BH = 0;
    OCR1BL = 50;
    sei();
}

void loop() {
}

