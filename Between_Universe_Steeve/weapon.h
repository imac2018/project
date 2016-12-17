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
	void shoot(Ship* parent, Space& world, QPointF origin);
	void activate();
	void stop();
	bool activated();

	class Bullet : public SpaceObject{
	protected:
		int life;
		int damage;
		Ship* parent;
	public:
		Bullet(Ship *parent, int life);
		virtual void animate();
		bool isDead() const;
		void damageEnemy(Enemy* e);
		void damageShip(Ship* e);
	};
	class LaserBullet : public Bullet{
		QLineF line;
		QPointF velocity;
	public:
		LaserBullet(Ship *parent, int life, float width, float vx);
		LaserBullet(Ship *parent, int life, float width, float vx, QPointF position);
		bool isInView(float x1, float x2) const;
		virtual void paint(QPainter& context) const;
		ColliderObject* collider() const;
		void animate();

		// SpaceObject interface
	public:
		void handleCollision(SpaceObject *o);
	};
};
#endif // WEAPON_H
