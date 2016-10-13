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

SOURCES += main.cpp\
        mainwindow.cpp \
    renderer.cpp

HEADERS  += mainwindow.h \
    renderer.h

FORMS    += mainwindow.ui
