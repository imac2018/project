#ifndef MODE_H
#define MODE_H

#include "renderer.h"
#include <QObject>

class StateStack;
class QInputEvent;
class QPaintEvent;

/**
 * @brief Define the current behavior of the game (level, menu, setting,...)
 */
class Mode : QObject
{

	Q_OBJECT

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
	/**
	 * @brief update the game at the start, when the mode is changing to this
	 * @param t float between 0 and 1, define the current time of the transition
	 */
	virtual bool updateStartTransition(float t) = 0;
	/**
	 * @brief update the game at the end, when the mode is changing to another
	 * @param t float between 0 and 1, define the current time of the transition
	 */
	virtual bool updateEndTransition(float t) = 0;
};

#endif // MODE_H
