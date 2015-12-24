#define EXTRABALL_PIN 8

bool gotExtraball;

void customInit() {
    _pins.setLow(EXTRABALL_PIN);
    gotExtraball = false;
}

void customEvent(int id) {
}

void customLoop() {
    if (_score >= 1000 && !gotExtraball) {
        _pins.setHigh(EXTRABALL_PIN);
        delay(100);
        _pins.setLow(EXTRABALL_PIN);
        gotExtraball = true;
    }
}

