#include "enemy.h"
#include "ship.h"
#include <memory>

Enemy::Enemy(float life, int damage, QPointF position, QPair<QPointF, QPointF> range)
	: life(life), position(position), range(range), damage(damage)
{}

bool Enemy::isDead() const
{
	return life <= 0 || position.x() < range.first.x() || position.x() > range.second.x();
}

void Enemy::handleCollision(SpaceObject *o)
{
	Ship* ship = dynamic_cast<Ship*>(o);
	if(ship != NULL){
		damageShip(ship);
	}
}

void Enemy::damageShip(Ship *ship)
{
	ship->life -= damage;
}

SquareEnemy::SquareEnemy(float life, int damage, const QPointF& position,
						 const QPair<QPointF, QPointF>& range, const QSizeF& size)
	: Enemy(life,damage,position,range), bound(position,size)
{
	c = QColor(55,255,10);

}

void SquareEnemy::paint(QPainter &context) const
{
	context.save();
	context.setPen(c);
	context.drawRect(bound);
	context.restore();
}

ColliderObject *SquareEnemy::collider() const
{
	std::unique_ptr<float> radius(new float);
	*radius = bound.width();
	return new ColliderRect(radius.get(),&position,&bound);
}

void SquareEnemy::animate()
{

}
