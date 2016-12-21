#ifndef WEAPON_H
#define WEAPON_H

#include <QLineF>
#include <QPointF>
#include "space.h"
#include <functional>
#define ShootFunctionDef(name) static void name(Weapon&, BulletCollisionListener& , Space&, QPointF)

class Enemy;
class Ship;
class Weapon;
class BulletCollisionListener;
typedef std::function<void(Weapon&, BulletCollisionListener&, Space&, QPointF)> ShootFunction;

/**
 * @brief implement this class to be allowed to instanciate a bullet and be notify when the
 * bullet has met something
 */
class BulletCollisionListener {
public:
	virtual bool handleBulletCollision(SpaceObject* o, float damage) = 0;
};

/**
 * @brief SpaceObject used to damage without direct collision
 */
class Bullet : public SpaceObject{
protected:
	int life;
	float damage;
	BulletCollisionListener& parent;
public:
	Bullet(BulletCollisionListener& parent, int life, QSizeF _size);
	virtual void animate();
	bool isDead() const;
};
class LaserBullet : public Bullet{
	QLineF line;
	QPointF velocity;
public:
	LaserBullet(BulletCollisionListener &parent, float damage, int life, float width, float vx,
				float vy);
	LaserBullet(BulletCollisionListener &parent, float damage, int life, float width, float vx,
				float vy, QPointF position);
	bool isInView(float x1, float x2) const;
	virtual void paint(QPainter& context) const;
	ColliderObject* collider() const;
	void animate();

	// SpaceObject interface
public:
	void handleCollision(SpaceObject *o);
};
class WaveBullet : public Bullet{
	QRectF rect;
	QPointF velocity;
	float inc;
public:
	WaveBullet(BulletCollisionListener &parent, float damage, int life, float width, float initialHeight,
			   float vx, float vy, float heightInc);
	WaveBullet(BulletCollisionListener &parent, float damage,
			   int life, float width, float initialHeight, float vx,
				float vy, float heightInc, QPointF position);
	bool isInView(float x1, float x2) const;
	virtual void paint(QPainter& context) const;
	ColliderObject* collider() const;
	void animate();

	// SpaceObject interface
public:
	void handleCollision(SpaceObject *o);
};

/**
 * @brief The Weapon is the class which is called to instantiate bullet.
 * You need to instanciate it with ShootFunctions (std::function<void(Weapon&,Ship*, Space&, QPointF)>),
 * these functions may create Bullet and add to Space. The current function used depend on the current
 * level
 */
class Weapon{

private:

	int level;
	int levelMax;
	QString _name;
	bool _activated;
	int currentDelay;
	QPointF position;
	int delayMax;
	QList<ShootFunction>::iterator currentShootFunction;
	QList<ShootFunction> shootFonctions;

public:
	//shoot function
	ShootFunctionDef(shootLaser1);
	ShootFunctionDef(shootLaser2);
	ShootFunctionDef(shootLaser3);
	ShootFunctionDef(shootLaser4);
	ShootFunctionDef(shootWave1);
	ShootFunctionDef(shootWave2);
	ShootFunctionDef(shootWave3);
	ShootFunctionDef(shootWave4);
	/**
	 *  @example Example:
	 *	void Weapon::shoot1(Weapon& w, Ship * parent, Space &world, QPointF origin){
	 *		world.addObject(new LaserBullet(parent, 1, 400,15,60,0, w.position + origin));
	 *	}
	 *  void Weapon::shoot2(Weapon& w, Ship * parent, Space &world, QPointF origin){
	 *		world.addObject(new LaserBullet(parent, 1.2, 400,20,70,0, w.position + origin));
	 *	}
	 *  Weapon w("name", QPointF(5,5),{&Weapon::shoot1, &Weapon::shoot2});
	 *  @param position relative to the parent of the weapon
	 */
	Weapon(QString name, QPointF position, QList<ShootFunction> shootFonctions);
	Weapon(QString name, QPointF position, int levelMax, ShootFunction shootFonctions[]);
	void shoot(BulletCollisionListener &parent, Space& world, QPointF origin);
	void activate();
	void stop();
	bool activated();
	void levelUp();

	QString name() const;

	virtual Weapon* instanciate(QPointF pos) const;
};
#endif // WEAPON_H
