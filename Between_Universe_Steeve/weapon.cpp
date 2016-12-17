#include "weapon.h"
#include "enemy.h"
#include "ship.h"
#include <QDebug>

Weapon::Weapon(QPointF position)
	: level(0), _activated(false), currentDelay(0), delayMax(10),
	  position(position)
{}

void Weapon::shoot(Ship *parent,Space &world, QPointF origin)
{
	if(!currentDelay){
		world.addObject(new LaserBullet(parent, 400,15,60, position + origin));
		currentDelay = delayMax;
	}
	else
		currentDelay--;
}

void Weapon::activate()
{
	currentDelay = 0;
	_activated = true;
}

void Weapon::stop()
{
	_activated = false;
}

bool Weapon::activated()
{
	return _activated;
}

Weapon::Bullet::Bullet(Ship* parent, int life)
	: parent(parent), life(life)
{

}

void Weapon::Bullet::animate()
{
	life -= 5;
}

bool Weapon::Bullet::isDead() const{
	return life <= 0;
}

void Weapon::Bullet::damageEnemy(Enemy *e)
{
	e->life -= damage;
}

Weapon::LaserBullet::LaserBullet(Ship* parent, int life, float width, float vx)
	: Bullet(parent, life), line(QPointF(0,0),QPointF(width,0)),
	  velocity(vx,0)
{
	this->damage = 10;
}

Weapon::LaserBullet::LaserBullet(Ship* parent, int life, float width, float vx, QPointF position)
	: LaserBullet(parent, life, width, vx)
{
	moveTo(position);
}

bool Weapon::LaserBullet::isInView(float x1, float x2) const
{
	return actualPosition().x() + line.x1() < x2 && actualPosition().x() + line.x2() > x1;
}

void Weapon::LaserBullet::paint(QPainter &context) const
{
	context.drawLine(line);
}

ColliderObject *Weapon::LaserBullet::collider() const
{
	QLineF line = this->line.translated(actualPosition());
	return new ColliderLine(QLine(line.x1(),line.y1(),
								  line.x2() + velocity.x() * 0.75,
								  line.y2() + velocity.y() * 0.75));
}

void Weapon::LaserBullet::animate()
{
	Bullet::animate();
	moveTo(actualPosition() + velocity);
}

void Weapon::LaserBullet::handleCollision(SpaceObject *o)
{
	Enemy* e = dynamic_cast<Enemy*>(o);
	if(e != NULL){
		damageEnemy(e);
		if(e->isDead()){
			parent->addPoints(e->points());
		}
		if(this->life>5)
			this->life = 5;
		velocity =(e->actualPosition() - actualPosition()) * 0.5;
		//moveTo(e->actualPosition().x());
	}
}
