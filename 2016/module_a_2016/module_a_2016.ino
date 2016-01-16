#define DISPLAY_LATCH_CLOCK_PIN 7
#define DISPLAY_DATA_PIN 8
#define DISPLAY_SHIFT_CLOCK_PIN 9


const int DISPLAY_DIGIT[] = {
//    gfedcba
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111  // 9
};

//                 cbadfge
#define DISPLAY_E B00111110
#define DISPLAY_R B00000110
#define DISPLAY_O B10010110

#define DISPLAY_DIGIT_COUNT 5

const int DISPLAY_ERROR_MESSAGE[] = { DISPLAY_R, DISPLAY_O, DISPLAY_R, DISPLAY_R, DISPLAY_E };

int getDigit(int digit) {
    return DISPLAY_DIGIT[digit];
}

bool _error = false;

void setup() {
    pinMode(DISPLAY_LATCH_CLOCK_PIN, OUTPUT);
    pinMode(DISPLAY_DATA_PIN, OUTPUT);
    pinMode(DISPLAY_SHIFT_CLOCK_PIN, OUTPUT);
}

void showNumber(unsigned long number) {
        digitalWrite(DISPLAY_LATCH_CLOCK_PIN, LOW);
        if (_error) {
            for (int i = 0; i < DISPLAY_DIGIT_COUNT; ++i) {
                shiftOut(DISPLAY_DATA_PIN, DISPLAY_SHIFT_CLOCK_PIN, MSBFIRST, DISPLAY_ERROR_MESSAGE[i]);
            }
        }
        else {
            long d = 10000;
            for (int i = 0; i < DISPLAY_DIGIT_COUNT; ++i) {
                int digit = getDigit((number / d) % 10);
                shiftOut(DISPLAY_DATA_PIN, DISPLAY_SHIFT_CLOCK_PIN, MSBFIRST, digit);
                d /= 10;
            }
        }
        delay(10);
        digitalWrite(DISPLAY_LATCH_CLOCK_PIN, HIGH);
        delay(10);
}
void loop() {
    showNumber(1234);
    delay(2000);
    showNumber(56789);
    delay(2000);
}
