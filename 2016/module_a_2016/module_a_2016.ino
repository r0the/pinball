
#include "display.h"
#include "sdcard.h"
#include "audio.h"

Display display;
SdCard sdcard;
Audio audio;

void setup() {
    display.setup();
    audio.setup();
    sdcard.setup();
}

void loop() {
    display.showText("HELLO");
    delay(3000);
//    display.showNumber(88888);
//    delay(3000);
    if (sdcard.status() == SD_NOT_PRESENT) {
        display.showText("INSRT");
        delay(3000);
        display.showText("CARD");
        delay(3000);
    }
    else {
//        audio.play("test.wav");
        pinMode(10, OUTPUT);
        display.showText("SD RDY");
        File file = SD.open("test.wav", FILE_READ);
        if (!file) {
            display.showText("READ");
            delay(3000);
        }

        unsigned char data; // Declares a variable to store data from the file.

        TCCR1B = (TCCR1B & 0b11111000) | 0x01;
        //TCCR0B = 0x01; // Sets the PWM frequency to 64KHz in pins 5 and 6.

        for(int count=0; count<129; count++) { // Jumps the header of the .wav file to access the data (after 129 readings). This number may also change for each file.
            data = file.read(); // Reads byte-by-byte until the end of the header.
        }

        while (file.available()) { // If the read data is greater or equal 0 (not null),
            data = file.read(); // Reads 1 byte of the file e stores it in "data"
            analogWrite(10, data);
            delayMicroseconds(40); // Waits for certain interval (in microseconds) for the next sample (according to the sample of the .wav file)
        }
    
        // This time waiting makes the samples being nearer the 8KHz frequency of the file.
    
        file.close(); // Closes the file.
    }
}

