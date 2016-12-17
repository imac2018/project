#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T16:07:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = Between_Universe
TEMPLATE = app

LIBS += opengl32.lib

SOURCES += *.cpp \
    space.cpp \
    colliderobject.cpp \
    enemy.cpp \
    weapon.cpp \
    items.cpp

HEADERS  += *.h \
    space.h \
    colliderobject.h \
    enemy.h \
    weapon.h \
    items.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressources.qrc
