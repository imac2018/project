#ifndef MODE_H
#define MODE_H

#include "renderer.h"
#include <QObject>

class StateStack;
class QInputEvent;
class QPaintEvent;

class Mode : QObject
{

	Q_OBJECT

	friend class StateStack;


public:
	Mode();
	const Renderer *renderer() const;
protected:
	Renderer* _renderer;

public:
	/**
	 * @fn inputHandle
	 * @brief Handle a input event
	 * @param event QInputEvent which may be a QKeyEvent, QMouseEvent or QWheelEvent
	 */
	virtual bool inputHandle(QInputEvent *event) = 0;

public slots:
	/**
	 * @brief update the game. May be used at each frame
	 * @return true if the draw function must be called to redraw the screen, false otherwise
	 */
	virtual bool update() = 0;
};

#endif // MODE_H
