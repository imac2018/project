#ifndef GUI_H
#define GUI_H

#include <QString>
#include <QPair>
#include <QRectF>
#include "object3d.h"

class Mode;
class Game;
class Renderer;
class Button;
class QMouseEvent;
class GuiElement;

class Gui{
	Button* hoveredButton;
	Button* pressedButton;
	QList<Button*> buttons;
	QList<GuiElement*> guiElements;
    QList<QPair<int,GuiElement*> > temporaryElements;
public:
	Gui();

	void appendBtn(Button* newBtn);
	void appendElement(GuiElement* elemnt);
	void appendTmpElement(int delay, GuiElement* elemnt);

	void addObjectsToRenderer(Renderer& r);

	bool handleMouseMove(QMouseEvent* mouseE);
	bool handleMousePress(QMouseEvent* mouseE);
	bool handleMouseRelease(QMouseEvent* mouseE);

	QList<Object3D*> objects() const;

	void render(Game &game) const;
	void update();

	void clear();

};

class GuiElement  : public Object3D{
public:
	bool invisible;

	GuiElement(QList<Vertex3D> vertex, const QColor &globalColor, int drawMode);
    GuiElement(std::vector<Vertex3D> vertex, const QColor &globalColor, int drawMode);
	void draw() const;
};

class Label : public GuiElement{
	QString text;
	QRectF bounds;
public:
	Label(Renderer &r, QString title, QRectF bounds, int fontsize,
		  QColor color=Qt::black, QColor border=Qt::white);
};

class Image : public GuiElement{
	QRectF bounds;
public:
	Image(Renderer &r, QRectF bounds, const QString &imagePath);
};

class DialogFrame : public GuiElement{
	QString text;
	QRectF bounds;
public:
	DialogFrame(Renderer &r, QString title, QRectF bounds, int fontsize);
};

class Button : public GuiElement{
	QString title;
	QRectF bounds;

	QOpenGLTexture * textureHover;
	QOpenGLTexture * texturePressed;
	QOpenGLTexture * textureInactiv;

	bool hover;
	bool pressed;
	//bool inactiv;

	void makeButtonTexture(Renderer &renderer, const QString& title,
						   float ratio, bool bold, int fontsize);
	void makeButtonTexture(Renderer &renderer, const QString& btnImagePath,
						   float ratio);

public:
	Button(Renderer &r, QString title, QRectF bounds, bool textonly, int fontSize);
	Button(Renderer &r, QString btnImagePath, QRectF bounds);
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
	ChangeModeButton(Game& g, Mode* next, Renderer &r, QString title, QRectF bounds,
					 bool textonly, int fontSize);
	ChangeModeButton(Game& g, Mode* next,Renderer &r, QString btnImagePath, QRectF bounds);
protected:
	virtual void action();
};

class ExitButton : public Button{
public:
	ExitButton(Renderer &r, QString title, QRectF bounds,
					 bool textonly, int fontSize);
	ExitButton(Renderer &r, QString btnImagePath, QRectF bounds);
protected:
	virtual void action();
};

struct Dialog;

class ToggleDialogButton : public Button{
	Dialog* parent;
	bool show;
public:
	ToggleDialogButton(Dialog* parent, bool show, Renderer &r, QString title, QRectF bounds,
					 bool textonly, int fontSize);
	ToggleDialogButton(Dialog* parent, bool show, Renderer &r, QString btnImagePath, QRectF bounds);
protected:
	void action();
};

struct Dialog{
	Button* cancel;
	Button* confirm;
	DialogFrame* frame;
	static int defaultBtnFontSize;

	Dialog();
	QRectF confirmBtnBounds() const;
	void initialize(Renderer &renderer, Button* confirm, QString message);
	void appendToGui(Gui& gui);
	void hide();
	void show();
	void showOkDialog();
	bool isVisible() const;
};

#endif // GUI_H
