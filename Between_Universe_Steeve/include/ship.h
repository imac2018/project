#ifndef SHIP_H
#define SHIP_H

#include <QPainter>
#include <QPointF>
#include <QImage>
#include "space.h"
#include "galata.h"
#include "item.h"

class Weapon;
class Enemy;
class Player;

/**
 *	group all the actions that a Ship can handle
 */
namespace ShipActions {
	void moveForward(Ship&);
	void moveBackward(Ship&);
	void moveUp(Ship&);
	void moveDown(Ship&);

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

class Bonus;

/**
 * @brief The Ship is a specific SpaceObject that can destroy Enemy and be destroy by it
 * This is the Player's object that allow his to play in a Galata
 */
class Ship : public SpaceObject, public BulletCollisionListener
{

	Player* parent;

	QString _name;

	float acceleration;
	float _power;
	int life;
	float defense;

	float deadState;

	QPointF velocity;

	int superModeDelay;
	int accelerateCounter;
	int powerUpCounter;

	float accelerateAmmount;
	float powerUpAmmount;

	Weapon* mainWeapon;
	Weapon* secondWeapon;

	QImage sprite;


	QList<QPointF> colliderBorder;
	QList<Bonus*> bonus;

public:
	Ship(Player* parent, const QString &_name, const QImage &sprite, float acceleration, float _power, int life, int superModeDelay,
		 float accelerateAmmount, float powerUpAmmount,
		 Weapon* mainWeapon = NULL, Weapon* secondWeapon = NULL);
	Ship(const QString& _name, const QImage& sprite, float acceleration,
		 float _power, int life, int superModeDelay,
		 float accelerateAmmount, float powerUpAmmount,
		 Weapon* mainWeapon = NULL, Weapon* secondWeapon = NULL);
	virtual void paint(QPainter& context) const;
	virtual void animate();
	virtual void handleDeath();
	virtual ColliderObject* collider() const;

	void moveForward();
	void moveBackward();
	void moveUp();
	void moveDown();

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
	void levelUp();

	void stopAccelerate();
	void stopPowerUp();

	friend void Bonus::addShipDefense(Ship& ship) const;
	friend void Bonus::removeShipDefense(Ship& ship) const;

	void addBonus(Bonus* b);

	void setMainWeapon(const Weapon &w);
	void setSecondWeapon(const Weapon& w);

	bool isDead() const;
	bool isDefinitelyDead() const;
	bool isInView(float x1, float x2) const;

	float power() const;
	QString name() const;

	void setParentPlayer(Player* parent);

	void addPoints(int points);

	/**
	 * @brief assert, throw an exception if the ship isn't
	 * correctly set
	 */
	void assert();

	virtual Ship* instanciate(Player *parent) const;

	void damageShip(Enemy&, int damage);

	bool handleBulletCollision(SpaceObject* o, float damage);
public:
	void handleCollision(SpaceObject *o);
};

#endif // SHIP_H
