#-------------------------------------------------
#
# Project created by QtCreator 2018-04-23T10:32:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = usb_can_qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canreceiver.cpp

HEADERS  += mainwindow.h\
         ControlCAN.h\
         resource.h \
    canreceiver.h \
    common.h

FORMS    += mainwindow.ui

INCLUDEPATH += . c:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include\
LIBS += -L. -lControlCAN
#LIBS += ControlCAN.lib


unix|win32: LIBS += -L$$PWD/ -lControlCAN

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
