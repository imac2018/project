#include "weapon.h"
#include "enemy.h"
#include <QDebug>

Weapon::Weapon(QPointF position)
	: level(0), _activated(false), currentDelay(0), delayMax(10),
	  position(position)
{}

void Weapon::shoot(Space &world, QPointF origin)
{
	if(!currentDelay){
		world.addObject(new LaserBullet(400,origin+position,15,60));
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

Weapon::Bullet::Bullet(int life)
	: life(life)
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

Weapon::LaserBullet::LaserBullet(int life, const QPointF& pos, float width, float vx)
	: Bullet(life), line(pos,pos+QPointF(width,0)), velocity(vx,0)
{

}

void Weapon::LaserBullet::paint(QPainter &context) const
{
	context.drawLine(line);
}

ColliderObject *Weapon::LaserBullet::collider() const
{
	return new ColliderLine(&line);
}

void Weapon::LaserBullet::animate()
{
	Bullet::animate();
	line.translate(velocity);
}

void Weapon::LaserBullet::handleCollision(SpaceObject *o)
{
	Enemy* e = dynamic_cast<Enemy*>(o);
	if(e != NULL){
		damageEnemy(e);
		this->life = 0;
		dynamic_cast<SquareEnemy*>(e)->c = QColor(255,0,0);
	}
}
