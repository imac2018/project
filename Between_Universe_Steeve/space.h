#ifndef SPACE_H
#define SPACE_H

#include <QList>
#include <QQueue>
#include <QPainter>
#include "colliderobject.h"

class Space;

class SpaceObject{
	friend class Space;
protected:
	Space* world;
public:
	SpaceObject() : world(NULL) {}
	void setWorld(Space* world);
	virtual void paint(QPainter& context) const = 0;
	virtual ColliderObject* collider() const = 0;
	virtual void animate() = 0;
	virtual bool isDead() const = 0;
	virtual void handleCollision(SpaceObject* o) = 0;
};

class Space
{
	QQueue<SpaceObject*> pendingItems;
	QList<SpaceObject*> items;
public:
	Space();
	void addObject(SpaceObject* object);
	void paint(QPainter& context) const;
	void animate();
};

#endif // SPACE_H
