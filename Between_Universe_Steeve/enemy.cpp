#include "enemy.h"
#include "ship.h"
#include <memory>
#include <QDebug>

Enemy::Enemy(float life, int damage, const PositionRange &range, int points)
	: life(life), range(range), damage(damage), deadState(0), _points(points)
{}

bool Enemy::isDead() const
{
	return life <= 0;
}

bool Enemy::isDefinitelyDead() const
{
	return life <= 0 && deadState >= 1.;
}

bool Enemy::isInView(float x1, float x2) const
{
	return range.first.x() < x2 && range.second.x() > x1;
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
	qDebug() << life << "\n";
	ship->life -= damage;
}

void Enemy::animate()
{
	if(behaviors.length()){
		QList<Behavior*>::Iterator i;
		QPointF pos = actualPosition();
		for(i = behaviors.begin(); i != behaviors.end();i++){
			pos = (*i)->move(pos);
		}
		moveTo(pos);
	}
}

void Enemy::addBehavior(Behavior *b)
{
	behaviors.append(b);
}

int Enemy::points() const
{
	return _points;
}

SquareEnemy::SquareEnemy(float life, int damage,
						 const PositionRange &range, const QSizeF& size)
	: Enemy(life,damage,range), size(size)
{
	c = QColor(55,255,10);

}

SquareEnemy::SquareEnemy(float life, int damage, const PositionRange &range,
						 const QSizeF &size, const QPointF &pos)
	: SquareEnemy(life,damage, range, size)
{
	moveTo(pos);
}

void SquareEnemy::paint(QPainter &context) const
{
	if(isDead()){
		float x = deadState - 0.5;
		x *= x;
		context.setOpacity(1-deadState);
		context.setPen(QColor(x*255,0,15));
	}
	else{
		context.setPen(c);
	}
	context.drawRect(QRect(0,0, size.width(), size.height()));
}

ColliderObject *SquareEnemy::collider() const
{
	return new ColliderRect(QRectF(actualPosition(), size));
}

void SquareEnemy::animate()
{
	Enemy::animate();
	if(isDead())
		deadState += 0.02f;
}

CircleEnemy::CircleEnemy(float life, int damage,
						 const PositionRange &range, float radius)
	: Enemy(life,damage,range), radius(radius)
{
	c = QColor(55,255,10);
}

CircleEnemy::CircleEnemy(float life, int damage, const PositionRange &range,
						 const float radius, const QPointF &position)
	: CircleEnemy(life,damage, range, radius)
{
	moveTo(position);
}

void CircleEnemy::paint(QPainter &context) const
{
	if(isDead()){
		drawExplosion(context, radius*0.5, deadState);
	}
	else{
		context.setPen(c);
		context.drawEllipse(QPointF(0,0),radius,radius);
	}

}

ColliderObject *CircleEnemy::collider() const
{
	return new ColliderCircle(radius,actualPosition());
}

void CircleEnemy::animate()
{
	Enemy::animate();
	if(isDead())
		deadState += 0.055f;
}
