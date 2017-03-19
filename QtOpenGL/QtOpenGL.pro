#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T01:40:40
#
#-------------------------------------------------

QT       += core gui opengl multimedia xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenGL
TEMPLATE = app


SOURCES += main.cpp\
    src/audio.cpp \
    src/camera.cpp \
    src/enemy.cpp \
    src/game.cpp \
    src/glexception.cpp \
    src/glwidget.cpp \
    src/gui.cpp \
    src/item.cpp \
    src/level.cpp \
    src/mainwindow.cpp \
    src/map.cpp \
    src/mapelement.cpp \
    src/menu.cpp \
    src/mode.cpp \
    src/object3d.cpp \
    src/player.cpp \
    src/renderer.cpp \
    src/tools.cpp

HEADERS  += include\mainwindow.h \
    include/audio.h \
    include/camera.h \
    include/enemy.h \
    include/game.h \
    include/glexception.h \
    include/glwidget.h \
    include/gui.h \
    include/item.h \
    include/level.h \
    include/mainwindow.h \
    include/map.h \
    include/mapelement.h \
    include/menu.h \
    include/mode.h \
    include/object3d.h \
    include/player.h \
    include/renderer.h \
    include/tools.h \
    include/vertex.h

INCLUDEPATH += $$PWD/include

RESOURCES += \
	ressource.qrc \
    assets_images.qrc

#unix|win32: LIBS += -L$$PWD/third-party/assimp/lib/ libassimp.dll.a
unix|win32: LIBS += -L$$PWD/third-party/assimp/lib/VC32 -lassimp

INCLUDEPATH += $$PWD/third-party/assimp/include
DEPENDPATH += $$PWD/third-party/assimp/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/third-party/assimp/lib/libassimp.dll.a
#else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/third-party/assimp/lib/liblibassimp.dll.a
