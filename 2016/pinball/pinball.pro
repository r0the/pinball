#-------------------------------------------------
#
# Project created by QtCreator 2016-06-05T06:12:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pinball
TEMPLATE = app


SOURCES += \
    Arduino.cpp \
    audio.cpp \
    display.cpp \
    logic.cpp \
    main.cpp\
    mainwindow.cpp \
    parser.cpp \
    vars.cpp \

HEADERS += \
    Arduino.h \
    audio.h \
    SD.h \
    consts.h \
    display.h \
    hardware.h \
    logic.h \
    mainwindow.h \
    parser.h \
    vars.h \

FORMS += \
    mainwindow.ui \

OTHER_FILES += \
    p.txt
