#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T01:40:40
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenGL
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
		glwidget.cpp \
		glexception.cpp \
		object3d.cpp \
		mode.cpp \
		game.cpp \
		renderer.cpp \
		camera.cpp \
		menu.cpp \
		tools.cpp \
		level.cpp \
		gui.cpp

HEADERS  += mainwindow.h \
		glwidget.h \
		glexception.h \
		object3d.h \
		tools.h \
		mode.h \
		game.h \
		renderer.h \
		camera.h \
		menu.h \
		vertex.h \
		level.h \
		gui.h

RESOURCES += \
	ressource.qrc

#unix|win32: LIBS += -L$$PWD/third-party/assimp/lib/ libassimp.dll.a
unix|win32: LIBS += -L$$PWD/third-party/assimp/lib/VC32 -lassimp

INCLUDEPATH += $$PWD/third-party/assimp/include
DEPENDPATH += $$PWD/third-party/assimp/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/third-party/assimp/lib/libassimp.dll.a
#else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/third-party/assimp/lib/liblibassimp.dll.a
