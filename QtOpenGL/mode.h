#ifndef MODE_H
#define MODE_H


class QInputEvent;
class Game;

/**
 * @brief Define the current behavior of the game (level, menu, setting,...)
 */
class Mode
{

public:

	/**
	 * @brief initialize the pipeline
	 * @return the state of the initialisation between 0 and 1
	 */
	virtual void initialize(Game&, float& currentState) = 0;

	/**
	 * @brief Handle a input event
	 * @param event QInputEvent which may be a QKeyEvent, QMouseEvent or QWheelEvent
	 */
	virtual bool inputHandle(Game &,QInputEvent *) = 0;

	/**
	 * @brief update the game. May be used at each frame
	 */
	virtual void update(Game &) = 0;

	/**
	 * @brief render the current frame
	 */
	virtual void render(Game&) const = 0;

	/**
	 * @brief clear the pipeline
	 */
	virtual void clear(Game&) = 0;
};

#endif // MODE_H
