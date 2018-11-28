QT += core
QT -= gui

TARGET = Hazard01
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    h55.cpp \
    hsetlowspeed.cpp \
    hsethighspeed.cpp \
    hlog.cpp

HEADERS += \
    h55.h \
    hsetlowspeed.h \
    hsethighspeed.h \
    hlog.h \
    HCommon.h

DISTFILES += \
    rawdata.txt

