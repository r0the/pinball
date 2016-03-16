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
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW); 
    digitalWrite(BLUE_PIN, LOW); 
    customLoop();
    delay(200);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH); 
    digitalWrite(BLUE_PIN, LOW); 
    customLoop();
    delay(200);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW); 
    digitalWrite(BLUE_PIN, HIGH); 
    customLoop();
    delay(200);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW); 
    digitalWrite(BLUE_PIN, LOW); 
    customLoop();
}

void setColor(byte r, byte g, byte b) {
    red = r;
    green = g;
    blue = b;
}

void customEvent(int id) {
    // Entry
    if (id == 1) {
        digitalWrite(RED_PIN, LOW); 
        digitalWrite(GREEN_PIN, LOW); 
        digitalWrite(BLUE_PIN, HIGH); 
    }

    // Multiball
    if (id == 2) {
        digitalWrite(RED_PIN, HIGH); 
        digitalWrite(GREEN_PIN, LOW); 
        digitalWrite(BLUE_PIN, LOW); 
    }

    // 
    if (id == 3) {
        digitalWrite(RED_PIN, LOW); 
        digitalWrite(GREEN_PIN, HIGH); 
        digitalWrite(BLUE_PIN, LOW); 
    }

    if (id == 4) {
        if (digitalRead(RED_PIN) == HIGH) {
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, LOW); 
            digitalWrite(BLUE_PIN, LOW); 
        }
        else {
            digitalWrite(RED_PIN, HIGH); 
            digitalWrite(GREEN_PIN, HIGH); 
            digitalWrite(BLUE_PIN, HIGH); 
        }
    }

    if (id == 5) {
            digitalWrite(RED_PIN, HIGH); 
            digitalWrite(GREEN_PIN, LOW); 
            digitalWrite(BLUE_PIN, HIGH); 
    }
    if (id == 6) {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW); 
        digitalWrite(BLUE_PIN, LOW); 
    }
}

void customLoop() {
}

