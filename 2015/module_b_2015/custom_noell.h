#define RED_PIN 1
#define GREEN_PIN 2
#define BLUE_PIN 3

byte red;
byte green;
byte blue;

void customInit() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    setColor(HIGH, LOW, LOW);
    customLoop();
    delay(200);
    setColor(LOW, HIGH, LOW);
    customLoop();
    delay(200);
    setColor(LOW, LOW, HIGH);    
    customLoop();
    delay(200);
    setColor(LOW, LOW, LOW);
    customLoop();
}

void setColor(byte r, byte g, byte b) {
    red = r;
    green = g;
    blue = b;
}

void customEvent(int id) {
    if (id == 1) {
        setColor(LOW, LOW, HIGH);
        _logic.addScore(200);
    }

    if (id == 2) {
        setColor(HIGH, LOW, LOW);
        
    }

    if (id == 3) {
        setColor(LOW, HIGH, LOW);
    }

    if (id == 4) {
        if (red || green || blue) {
            setColor(LOW, LOW, LOW);
        }
        else {
            setColor(HIGH, HIGH, HIGH);
        }
        _logic.addScore(50);
    }

    if (id == 5) {
//        _score += 155;
    }

    if (id == 6) {
        setColor(LOW, LOW, LOW);
    }
}

void customLoop() {
    digitalWrite(RED_PIN, red); 
    digitalWrite(GREEN_PIN, green); 
    digitalWrite(BLUE_PIN, blue); 
}

