#ifndef ENEMY_H
#define ENEMY_H

#include "space.h"
#include "weapon.h"
#include <QPair>

class Enemy : public SpaceObject
{
protected:
	float life;
	int damage;
	QPointF position;
	QPair<QPointF, QPointF> range;
public:
	Enemy(float life, int damage, QPointF position, QPair<QPointF, QPointF> range);

	// SpaceObject interface
public:
	bool isDead() const;
	void handleCollision(SpaceObject *o);
	void damageShip(Ship* ship);
	friend void Weapon::Bullet::damageEnemy(Enemy* e);
};

class SquareEnemy : public Enemy{
	QRectF bound;
public:
	QColor c;
	SquareEnemy(float life, int damage, const QPointF &position,
				const QPair<QPointF, QPointF> &range, const QSizeF &size);


	// SpaceObject interface
public:
	void paint(QPainter &context) const;
	ColliderObject *collider() const;
	void animate();
};

#endif // ENEMY_H
