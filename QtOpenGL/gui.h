#ifndef GUI_H
#define GUI_H

#include <QString>
#include <QRectF>
#include "object3d.h"

class Mode;
class Game;
class Renderer;
class Button;
class QMouseEvent;

class Gui{
	Button* hoveredButton;
	Button* pressedButton;
	QList<Button*> buttons;
public:
	Gui();

	void append(Button* newBtn);

	bool handleMouseMove(QMouseEvent* mouseE);
	bool handleMousePress(QMouseEvent* mouseE);
	bool handleMouseRelease(QMouseEvent* mouseE);

	QList<Object3D*> objects() const;

};

class Label : public Object3D{
	QString text;
	QRectF bounds;
public:
	Label(Renderer &r, QString title, QRectF bounds);
};

class Image : public Object3D{
	QRectF bounds;
public:
	Image(Renderer &r, QRectF bounds, const QString &imagePath);
};

class Button : public Object3D{
	QString title;
	QRectF bounds;

	QOpenGLTexture * textureHover;
	QOpenGLTexture * texturePressed;
	QOpenGLTexture * textureInactiv;

	bool hover;
	bool pressed;
	bool invisible;
	bool inactiv;

	void makeButtonTexture(Renderer &renderer, const QString& title, float ratio);

public:
	Button(Renderer &r, QString title, QRectF bounds);
	bool containsPoint(QPointF point);
	void pressedEvent();
	void releaseEvent(bool action);
	void hoverEvent();
	void leaveEvent();
	void draw() const;

protected:
	virtual void action();
};

class ChangeModeButton : public Button{
	Mode* next;
	Game& game;
public:
	ChangeModeButton(Game& g, Mode* next, Renderer &r, QString title, QRectF bounds);
protected:
	virtual void action();
};

#endif // GUI_H
