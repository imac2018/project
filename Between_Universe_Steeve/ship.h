#ifndef SHIP_H
#define SHIP_H

#include <QPainter>
#include <QPointF>
#include "space.h"
#include "galata.h"
#include <QImage>
#include "enemy.h"

class Weapon;
// Coucou

namespace ShipActions {
	void moveForward(Ship&);
	void moveBackward(Ship&);
	void moveUp(Ship&);
	void moveDown(Ship&);
//ca va?
	void stopForward(Ship&);
	void stopBackward(Ship&);
	void stopUp(Ship&);
	void stopDown(Ship&);

	void mainWeaponActivate(Ship&);
	void secondWeaponActivate(Ship&);

	void mainWeaponStop(Ship&);
	void secondWeaponStop(Ship&);

	void accelerate(Ship&);
	void powerUp(Ship&);


}

class Ship : SpaceObject
{
	QPointF position;
	QRectF bound;

	float acceleration;
	float power;
	float life;

	QPointF velocity;

	int superModeDelay;
	int accelerateCounter;
	int powerUpCounter;

	float accelerateAmmount;
	float powerUpAmmount;

	Weapon* mainWeapon;
	Weapon* secondWeapon;

	QImage sprite;

	void stopAccelerate();
	void stopPowerUp();

public:

	Ship(float speed, float power, float life, int superModeDelay,
		 float accelerateAmmount, float powerUpAmmount,
		 Weapon* mainWeapon, Weapon* secondWeapon);
	virtual void paint(QPainter& context) const;
	virtual void animate();
	virtual ColliderObject* collider() const;

	void moveForward();
	void moveBackward();
	void moveUp();
	void moveDown();
//déplacement
	void stopForward();
	void stopBackward();
	void stopUp();
	void stopDown();

	void mainShootActivate();
	void secondShootActivate();

	void mainShootStop();
	void secondShootStop();


	void accelerate();
	void powerUp();

	bool isDead() const;

	friend void Galata::addShip(Ship* ship);
	friend void Enemy::damageShip(Ship* ship);
	// SpaceObject interface
public:
	void handleCollision(SpaceObject *o);
};



#endif // SHIP_H
