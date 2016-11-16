#ifndef WEAPON_H
#define WEAPON_H

#include <QLineF>
#include <QPointF>
#include "space.h"

class Enemy;
class Ship;

class Weapon{
	int level;
	bool _activated;
	int currentDelay;
	QPointF position;
	int delayMax;
public:
	Weapon(QPointF position);
	void shoot(Space& world, QPointF origin);
	void activate();
	void stop();
	bool activated();

	class Bullet : public SpaceObject{
	protected:
		int life;
		int damage;
	public:
		Bullet(int life);
		virtual void animate();
		bool isDead() const;
		void damageEnemy(Enemy* e);
		void damageShip(Ship* e);
	};
	class LaserBullet : public Bullet{
		QLineF line;
		QPointF velocity;
	public:
		LaserBullet(int life, const QPointF& pos, float width, float vx);
		virtual void paint(QPainter& context) const;
		ColliderObject* collider() const;
		void animate();

		// SpaceObject interface
	public:
		void handleCollision(SpaceObject *o);
	};
};
#endif // WEAPON_H
