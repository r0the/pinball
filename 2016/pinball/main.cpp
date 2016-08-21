#include "mainwindow.h"
#include <QApplication>
#include "logic.h"
#include "vars.h"
#include "iopins.h"
#include "display.h"

#define CHAR_SPACE 10
#define CHAR_MINUS 11
#define CHAR_A     16
#define CHAR_B     17
#define CHAR_C     18
#define CHAR_D     19
#define CHAR_E     20
#define CHAR_F     21
#define CHAR_G     22
#define CHAR_H     23
#define CHAR_I     24
#define CHAR_J     25
#define CHAR_K     26
#define CHAR_L     27
#define CHAR_M     28
#define CHAR_N     29
#define CHAR_O     30
#define CHAR_P     31
#define CHAR_Q     32
#define CHAR_R     33
#define CHAR_S     34
#define CHAR_T     35
#define CHAR_U     36
#define CHAR_V     37
#define CHAR_W     38
#define CHAR_X     39
#define CHAR_Y     40
#define CHAR_Z     41


static uint8_t convertChar(char ch) {
    if (ch == '-') {
        return CHAR_MINUS;
    }
    else if ('0' <= ch && ch <= '9') {
        return ch - '0';
    }
    else if ('A' <= ch && ch <= 'Z') {
        return CHAR_A + ch - 'A';
    }
    else {
        return CHAR_SPACE;
    }
}

uint32_t encode(const char* text) {
    uint32_t value = 0;
    for (int i = 0; i < 5; ++i) {
        value += convertChar(text[4-i]);
        if (i < 4) {
            value <<= 6;
        }
    }

    qDebug() << QString::number(value, 16);
    return value;
}

int main(int argc, char *argv[])
{
    IoPins pins;
    Vars vars(pins);
    Display display;
    Logic logic(display, vars);
    logic.setup();
    qDebug() << "IO-A";
    logic.handleEvent(EVENT_IN_A);
    qDebug() << "IO-A";
    logic.handleEvent(EVENT_IN_A);
    qDebug() << "IO-A";
    logic.handleEvent(EVENT_IN_A);
    qDebug() << "IO-A";
    logic.handleEvent(EVENT_IN_A);
    qDebug() << "IO-A";
    logic.handleEvent(EVENT_IN_A);
    qDebug() << "IO-A";
    logic.handleEvent(EVENT_IN_A);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
