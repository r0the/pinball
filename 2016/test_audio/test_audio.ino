/*
 * PCM audio playback on Pin 10 of an Arduino Uno
 * 
 * Pin 10 maps to PB2 / SS / OC1B / PCINT2 pin of the ATmega328
 *
 * 
 */

#include <Arduino.h>
#include <SD.h>

uint8_t wgm = 1;
uint8_t cs = 1;
volatile uint16_t index = 0;
volatile uint8_t divisor = 0;

#define BUFFER_SIZE 256

uint8_t buffer[2][BUFFER_SIZE];
volatile uint8_t activeBuffer = 0;
volatile uint8_t loadBuffer = 1;
volatile uint16_t bufferPos = 0;
File file;

ISR(TIMER1_OVF_vect) {
    if (divisor < 1) {
        ++divisor;
    }
    else {
        divisor = 0;
        OCR1BL = buffer[activeBuffer][bufferPos];
        ++bufferPos;
        if (bufferPos >= BUFFER_SIZE) {
            activeBuffer = (activeBuffer + 1) % 2;
            bufferPos = 0;
        }
    }
}

void setup() {
    cli();
    SD.begin(1);
    file = SD.open("test.wav", FILE_READ);

    pinMode(10, OUTPUT);
    // Setup timer 1

    // TCCR1B - Timer/Counter1 Control Register B
    //
    // | Bits  | Name           | Value | Meaning                       |
    // | ----- | -------------- | ----- | ----------------------------- |
    // | 7     | ICNC1          | 0     | irrelevant                    |
    // | 6     | ICES1          | 0     | irrelevant                    |
    // | 5     | -              | 0     | -                             |
    // | 4/3   | WGM13/WGM12    |       | Waveform generation mode      |
    // | 2/1/0 | CS12/CS11/CS10 |       | Clock select /64              |

    TCCR1B = ((wgm & 0x0C) << 1) | (cs & 0x07);

    // TCCR1A - Timer/Counter1 Control Register A
    //
    // | Bits  | Name          | Value | Meaning                       |
    // | ----- | ------------- | ----- | ----------------------------- |
    // | 7/6   | COM1A1/COM1A0 | 00    | OC1A (Pin 9) disconnected     |
    // | 5/4   | COM1B1/COM1B0 | 10    | OC1B (Pin 10) clear on match  |
    // | 3/2   | -             | 00    | -                             |
    // | 1/0   | WGM11/WGM10   | 00    | Waveform generation mode      |

    TCCR1A = B00100000 | (wgm & 0x03);

    // TIMSK1 - Timer/Counter1 Interrupt Mask Register
    //
    // | Bits  | Name           | Value | Meaning                       |
    // | ----- | -------------- | ----- | ----------------------------- |
    // | 7/6   | -              | 00    | -                             |
    // | 5     | ICIE1          | 1     | enable timer 1 interrupt      |
    // | 4/3   | -              | 00    | -                             |
    // | 2/1   | OCIE1B/OCIE1A  | 00    | irrelevant                    |
    // | 0     | TOIE1          | 1     | enable interrupt on top       |

    TIMSK1 = B00100001;

    // ICR1H/ICR1L - Input Capture Register 1
    //
    // This register is used for defining the counter TOP value
    // 
    ICR1H = 0x0000;
    ICR1L = 0x00FF;
    OCR1BH = 0;
    OCR1BL = 250;
    checkBuffer();
    sei();
}

void checkBuffer() {
    cli();
    if (activeBuffer != loadBuffer) {
        loadBuffer = (loadBuffer + 1) % 2;
        uint16_t size = file.read(reinterpret_cast<uint8_t*>(buffer[loadBuffer]), BUFFER_SIZE);
    }
    sei();
}

void loop() {
    checkBuffer();
}

