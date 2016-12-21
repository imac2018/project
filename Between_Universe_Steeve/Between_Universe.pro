#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T16:07:52
#
#-------------------------------------------------

QT		+= core gui xml
CONFIG	+= c++11
QMAKE_CXXFLAGS_RELEASE += -O2


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = Between_Universe
TEMPLATE = app

INCLUDEPATH += ./include

LIBS += opengl32.lib

FORMS    += mainwindow.ui \
    configdialog.ui

RESOURCES += \
    ressources.qrc

HEADERS += \
    include/character.h \
    include/colliderobject.h \
    include/configdialog.h \
    include/enemy.h \
    include/galata.h \
    include/game.h \
    include/glwidget.h \
    include/item.h \
    include/mainwindow.h \
    include/mode.h \
    include/player.h \
    include/renderer.h \
    include/ship.h \
    include/space.h \
    include/weapon.h \
    include/widget.h

SOURCES += \
	main.cpp \
    src/character.cpp \
    src/colliderobject.cpp \
    src/configdialog.cpp \
    src/enemy.cpp \
    src/galata.cpp \
    src/game.cpp \
    src/item.cpp \
    src/mainwindow.cpp \
    src/mode.cpp \
    src/player.cpp \
    src/renderer.cpp \
    src/ship.cpp \
    src/space.cpp \
    src/weapon.cpp \
    src/glwidget.cpp \
    src/widget.cpp
