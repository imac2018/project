#ifndef ENEMY_H
#define ENEMY_H

#include "space.h"
#include "weapon.h"

class Enemy : public SpaceObject
{
protected:
	float life;
	int damage;
	int _points;
	QPair<QPointF, QPointF> range;
	float deadState;
	QList<Behavior*> behaviors;
public:
	Enemy(float life, int damage, const PositionRange& range, int points = 10);

	// SpaceObject interface
public:
	bool isDead() const;
	bool isDefinitelyDead() const;
	bool isInView(float x1, float x2) const;
	void handleDead() const;
	void handleCollision(SpaceObject *o);
	void damageShip(Ship* ship);
	void animate();
	void addBehavior(Behavior* b);
	int points() const;
	friend void Weapon::Bullet::damageEnemy(Enemy* e);
};

class SquareEnemy : public Enemy{
	QSizeF size;
public:
	QColor c;
	SquareEnemy(float life, int damage, const PositionRange &range,
				const QSizeF &size);
	SquareEnemy(float life, int damage, const PositionRange &range,
				const QSizeF &size, const QPointF &pos);


	// SpaceObject interface
public:
	void paint(QPainter &context) const;
	ColliderObject *collider() const;
	void animate();
};

class CircleEnemy : public Enemy{
	float radius;
public:
	QColor c;
	CircleEnemy(float life, int damage, const PositionRange &range,
				const float radius);
	CircleEnemy(float life, int damage, const PositionRange &range,
				 const float radius, const QPointF& position);


public:
	void paint(QPainter &context) const;
	ColliderObject *collider() const;
	void animate();
};

#endif // ENEMY_H
