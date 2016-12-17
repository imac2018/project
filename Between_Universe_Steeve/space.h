#ifndef SPACE_H
#define SPACE_H

#include <QList>
#include <QQueue>
#include <QSet>
#include <QPainter>
#include "colliderobject.h"

#include <QPair>
#define NoPen QPen(QBrush(),0)

typedef QPair<QPointF, QPointF> PositionRange;

class Space;

class SpaceObject{
	friend class Space;
private:
	QPointF _actualPosition;
	QPointF _requestedPosition;
	bool _updated;
	void setActualPosition(QPointF pos);
protected:
	Space* world;

public:
	SpaceObject() : _updated(true), _actualPosition(5,5) {}
	void moveTo(const QPointF& p);
	bool updated();
	void setWorld(Space* world);
	virtual void paint(QPainter& context) const = 0;
	virtual ColliderObject* collider() const = 0;
	virtual void animate() = 0;
	virtual bool isDead() const = 0;
	virtual bool isDefinitelyDead() const;
	virtual bool isInView(float x1, float x2) const = 0;
	virtual void handleCollision(SpaceObject* o) = 0;
	virtual QPointF actualPosition() const;
	virtual QPointF requestedPosition() const;

	static void drawExplosion(QPainter &context, float radius, float state);
};

class Space
{
	QList<SpaceObject*> pendingItems;
	QList<SpaceObject*> items;
	QSet<SpaceObject*> blockedItems;
	float viewX;
	QSizeF viewSize;
	float scale;
	float velocity;

	void setPosition(SpaceObject& object);
public:
	Space();
	Space(float viewX, float viewWidth, float viewHeight, float velocity = 1);
	void addObject(SpaceObject* object);
	void addBlockedObject(SpaceObject* object);
	void paint(QPainter& context) const;
	void animate();
	void resizeHandler(float windowWidth, float windowHeight);
	QRectF viewBound() const;

};

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
	QPointF move(QPointF initial);
};

class B_sinusoidal : public Behavior{
protected:
	float t;
	float cosInc;
	float sinInc;
	float cosT;
	float sinT;
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
