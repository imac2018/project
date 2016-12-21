#ifndef SPACE_H
#define SPACE_H

#include <QList>
#include <QQueue>
#include <QSet>
#include <QPainter>
#include "colliderobject.h"

#include <QPair>

typedef QPair<QPointF, QPointF> PositionRange;

class Space;

/**
 * @brief Base class for any object in a Space. If you want to make a object which may appeared
 * in a level, you should make a subclass of it
 */
class SpaceObject{
	friend class Space;
private:
	QPointF _actualPosition;
	QPointF _requestedPosition;

	bool _updated;
	void setActualPosition(QPointF pos);
protected:
	Space* world;
	QSizeF _size;
public:
	SpaceObject(QSizeF _size);
	/**
	 * @brief moveTo Ask to the Space to move at this position
	 */
	void moveTo(const QPointF& p);
	/**
	 * @return true if the Space moved the object since the last position request
	 */
	bool updated();
	void setWorld(Space* world);
	virtual void paint(QPainter& context) const = 0;
	/**
	 * @return the collider which may be used for the collisions tests
	 */
	virtual ColliderObject* collider() const = 0;
	virtual void animate() = 0;
	/**
	 * @return true if the object is dying and may be not used while the collisions tests
	 */
	virtual bool isDead() const = 0;
	/**
	 * @return true if the object is definitely dead and should be removed from the Space
	 */
	virtual bool isDefinitelyDead() const;
	/**
	 * @brief function called when the object is removed from the Space due to its death
	 */
	virtual void handleDeath();
	/**
	 * @param x1 left border
	 * @param x2 right border
	 * @return true if the object should be in the space when the screen is at this position
	 */
	virtual bool isInView(float x1, float x2) const = 0;
	/**
	 * @brief function called when the object met another
	 */
	virtual void handleCollision(SpaceObject* o) = 0;
	QPointF actualPosition() const;
	QPointF requestedPosition() const;
	QSizeF size() const;

	static void drawExplosion(QPainter &context, float radius, float state);
};

/**
 * @brief The Space is the "world" class, it manage all physical object.
 * It draw and update all objects it have and make the collisions tests. Also it move forward at each update
 */
class Space
{
	QList<SpaceObject*> pendingItems;
	QList<SpaceObject*> items;
	QSet<SpaceObject*> blockedItems;
	float viewX;
	float viewXEnd;
	QSizeF viewSize;
	float scale;
	float velocity;
	QImage background;

	void setPosition(SpaceObject& object);
public:
	Space();
	/**
	 * @brief Space
	 * @param background
	 * @param viewX initial x position
	 * @param viewWidth viewport width
	 * @param viewHeight viewport height
	 * @param xEnd When viewport pass this value the level should be over
	 * @param velocity the speed with which the viewport move
	 */
	Space(QImage background, float viewX, float viewWidth,
		  float viewHeight, float xEnd, float velocity = 1);
	~Space();
	void addObject(SpaceObject* object);
	/**
	 * @brief The blocked object can't go out the viewport
	 */
	void addBlockedObject(SpaceObject* object);
	void paint(QPainter& context) const;
	void animate();
	/**
	 * @brief rescale the draw viewport to have the correct ratio
	 */
	void resizeHandler(float windowWidth, float windowHeight);
	QRectF viewBound() const;
	bool isEnded() const;

	/**
	 * @brief scale and translate the context to suit the viewport
	 */
	void applyTransform(QPainter& context, float windowWidth, float windowHeight);

	static const QPen noPen;
	static const QBrush noBrush;
	static const QPointF origin;

};


/**
 * @brief The Behavior is an interface which may be used to perform a specific move
 * subclass it to get a custom move
 */
class Behavior{
public:
	virtual QPointF move(QPointF initial) = 0;
};

class B_TurnAround : public Behavior{
	float rotateSpeedX;
	float rotateSpeedY;
	float rotateValueX;
	float rotateValueY;
	float distance;
public:
	B_TurnAround(float angleSpeed, float distance, float initialAngle);
	QPointF move(QPointF initial);
};

class B_Translate : public Behavior{
protected:
	QPointF translation;
public:
	B_Translate(QPointF translation);
	QPointF move(QPointF initial);
};

class B_TranslateAlternate : public B_Translate{
protected:
	QPair<int,int> range;
	int state;
	int inc;
public:
	B_TranslateAlternate(QPair<float,float> range, QPointF translation);
	B_TranslateAlternate(float tstart, float tend, QPointF translation);
	QPointF move(QPointF initial);
};

class B_sinusoidal : public Behavior{
protected:
	float t;
	float cosInc;
	float sinInc;
	float cosT;
	float sinT;
	float coefC, coefS;
	float module;
public:
	B_sinusoidal(float phase, float module, float coefCos, float coefSin);
	QPointF move(QPointF initial);
};

class B_Temporal : public Behavior{
protected:
	Behavior* behavior;
	int t;
	int t_start;
	int t_end;
public:
	B_Temporal(Behavior* b, int t_start, int t_end);
	QPointF move(QPointF initial);
};

class B_TemporalCycle : public B_Temporal{
protected:
	int cycle;
public:
	B_TemporalCycle(Behavior* b, int t_start, int t_end, int cycle);
	QPointF move(QPointF initial);
};

typedef QList<QPair<Behavior*, int> > LimitedBehaviors;

class B_String : public Behavior{
protected:
	QList<QPair<Behavior*, int> > behaviors;
	int t;
	QList<QPair<Behavior*, int> >::iterator current;
public:
	B_String();
	B_String(LimitedBehaviors behaviors);
	void addBehavior(QPair<Behavior*, int> behavior);
	void addBehavior(Behavior* behavior, int duration);
	QPointF move(QPointF initial);
};

class B_StringCycle : public B_String{
protected:
	int delayBetweenCycle;
public:
	B_StringCycle(int delayBetweenCycle = 0);
	B_StringCycle(LimitedBehaviors behaviors, int delayBetweenCycle = 0);
	QPointF move(QPointF initial);
};

#endif // SPACE_H
