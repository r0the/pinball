#-------------------------------------------------
#
# Project created by QtCreator 2016-06-05T06:12:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pinball
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    iopins.cpp \
    logic.cpp \
    Arduino.cpp

HEADERS  += mainwindow.h \
    hardware.h \
    iopins.h \
    logic.h \
    Arduino.h \
    SD.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    actions.txt
