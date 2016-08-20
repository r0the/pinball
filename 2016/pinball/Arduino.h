#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtGlobal>
#include <QFile>
#include <QDebug>

typedef quint8 uint8_t;
typedef quint16 uint16_t;
typedef quint32 uint32_t;

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

#define PROGMEM

#define FILE_READ QIODevice::ReadOnly

void pinMode(quint8 pin, quint8 mode);

void digitalWrite(uint8_t pin, uint8_t value);

uint32_t millis();

uint16_t digitalRead(uint8_t pin);

class File {
public:
    File(QFile* handle) :
        _handle(handle) {
    }

    ~File() {
        delete _handle;
    }

    qint64 available() const {
        return _handle->bytesAvailable();
    }

    void close() {
        _handle->close();
    }

    char read() {
        char result;
        _handle->getChar(&result);
        qDebug() << "Reading " << result;
        return result;
    }

private:
    QFile* _handle;
};

#endif // ARDUINO_H
