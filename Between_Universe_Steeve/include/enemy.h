#ifndef ENEMY_H
#define ENEMY_H

#include "space.h"
#include "weapon.h"
#include "item.h"
#include "ship.h"
#include <QtXml>

/**
 * @brief Enemy is a SpaceObject which can be destroyed by a ship
 * It give points to the ship which destroyed it and can drop some Items
 * It have some Behaviors which define its moves
 */
class Enemy : public SpaceObject
{
protected:
	int life;
	int damage;
	int _points;
	QPair<QPointF, QPointF> range;
	float deadState;
	QList<Behavior*> behaviors;
	QList<Item*> droppedItems;
public:
	Enemy(int life, int damage, QSizeF _size,
		  const PositionRange& range, int points = 10);
	~Enemy();

	// SpaceObject interface
public:
	bool isDead() const;
	bool isDefinitelyDead() const;
	bool isInView(float x1, float x2) const;
	void handleCollision(SpaceObject *o);
	void handleDeath();
	void animate();
	void addBehavior(Behavior* b);
	void addDroppedItems(Item* i);
	int points() const;

	void damageEnemy(Ship &ship, int damage);
};

class SquareEnemy : public Enemy{
public:
	QColor color;
	bool fill;
	QImage sprite;
	SquareEnemy(int life, int damage,
				const QSizeF &_size, QColor c, bool fill,
				const PositionRange &range);
	SquareEnemy(int life, int damage,
				const QSizeF &_size, QColor c, bool fill,
				const PositionRange &range, const QPointF &pos);
	SquareEnemy(int life, int damage,
				const QSizeF &size, QImage sprite,
				const PositionRange &range);
	SquareEnemy(int life, int damage,
				const QSizeF &size, QImage sprite,
				const PositionRange &range, const QPointF &pos);


	// SpaceObject interface
public:
	void paint(QPainter &context) const;
	ColliderObject *collider() const;
	void animate();
};

class CircleEnemy : public Enemy{
	float radius;
public:
	QColor color;
	bool fill;
	QImage sprite;
	CircleEnemy(int life, int damage,const float radius,
				QColor c, bool fill, const PositionRange &range);
	CircleEnemy(int life, int damage, const float radius,
				QColor c, bool fill, const PositionRange &range,
				const QPointF& position);
	CircleEnemy(int life, int damage, const float radius,QImage sprite,
				const PositionRange &range);
	CircleEnemy(int life, int damage, const float radius,QImage sprite,
				const PositionRange &range, const QPointF& position);


public:
	void paint(QPainter &context) const;
	ColliderObject *collider() const;
	void animate();
};

#endif // ENEMY_H
