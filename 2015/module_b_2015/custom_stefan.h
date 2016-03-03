#define LAMP_PIN 8
#define MAX_BLINK
#define LAMP_ON_MILLIS 200
#define LAMP_OFF_MILLIS 500

unsigned long _blinkCount;
unsigned long _lampTimeout;
bool _lampOn;

void customInit() {
    _pins.setLow(LAMP_PIN);
    _blinkCount = 0;
    _lampTimeout = 0;
}

void customEvent(int id) {
    if (id == 7) {
        _blinkCount = 5;
        _logic.setScore(_logic.score() * 2);
        _lampOn = false;
    }
}

void customLoop() {
    unsigned long now = millis();
    if (_blinkCount > 0 && _lampTimeout < now) {
        if (_lampOn) {
          _lampTimeout = now + LAMP_OFF_MILLIS;
          _pins.setLow(LAMP_PIN);
          _lampOn = false;
          _blinkCount = _blinkCount - 1;
        }
        else {
          _lampTimeout = now + LAMP_ON_MILLIS;
          _pins.setHigh(LAMP_PIN);
          _lampOn = true;
        }
    }
}

