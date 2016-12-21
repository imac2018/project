#include "enemy.h"
#include "ship.h"
#include <memory>
#include <QtXml>

Enemy::Enemy(int life, int damage, QSizeF size,
			 const PositionRange &range, int points)
	: SpaceObject(size),
	  life(life), range(range), damage(damage), deadState(0), _points(points)
{}

Enemy::~Enemy()
{
	QMutableListIterator<Behavior*> removerI(behaviors);
	while(removerI.hasNext()){
		delete removerI.next();
		removerI.remove();
	}

}

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
	if(range.first.x()<0)
		return true;
	else
		return range.first.x() < x2 && range.second.x() > x1;
}

void Enemy::handleCollision(SpaceObject *o)
{
	Ship* ship = dynamic_cast<Ship*>(o);
	if(ship != NULL){
		ship->damageShip(*this, damage);
	}
}

void Enemy::damageEnemy(Ship& s, int damage)
{
	life -= damage;
	if(isDead()){
		s.addPoints(points());
	}
}

void Enemy::handleDeath()
{
	QPointF spawn(actualPosition());
	foreach (Item* i, droppedItems) {
		i->moveTo(actualPosition());
		world->addObject(i);
		spawn.rx() += 2;
	}
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

void Enemy::addDroppedItems(Item *i)
{
	droppedItems.append(i);
}

int Enemy::points() const
{
	return _points;
}

SquareEnemy::SquareEnemy(int life, int damage, const QSizeF& size, QColor c,
						 bool fill, const PositionRange &range)
	: Enemy(life,damage,size,range), color(c), fill(fill)
{
}

SquareEnemy::SquareEnemy(int life, int damage,	 const QSizeF &size, QColor c,
						 bool fill, const PositionRange &range,
						 const QPointF &pos)
	: SquareEnemy(life,damage, size,c,fill, range)
{
	moveTo(pos);
}

SquareEnemy::SquareEnemy(int life, int damage, const QSizeF &size,
						 QImage sprite, const PositionRange &range)
	: Enemy(life, damage, size, range),
	  sprite(sprite.scaledToWidth(size.width()))
{}

SquareEnemy::SquareEnemy(int life, int damage, const QSizeF &size,
						 QImage sprite, const PositionRange &range,
						 const QPointF &pos)
	: SquareEnemy(life,damage,size,sprite,range)
{
	moveTo(pos);
}

void SquareEnemy::paint(QPainter &context) const
{

	if(sprite.isNull()){
		if(fill)
			context.setBrush(color);
		else
			context.setBrush(Space::noBrush);
		context.setPen(color);
		context.drawRect(QRect(0,0, _size.width(), _size.height()));
	}
	else{
		context.drawImage(0,0,sprite);
	}
	if(isDead()){
		drawExplosion(context, _size.width()*0.5, deadState);
	}

}

ColliderObject *SquareEnemy::collider() const
{
	return new ColliderRect(QRectF(Space::origin, _size));
}

void SquareEnemy::animate()
{
	Enemy::animate();
	if(isDead())
		deadState += 0.07f;
}

CircleEnemy::CircleEnemy(int life, int damage,
						 float radius, QColor c, bool fill,
						 const PositionRange &range)
	: Enemy(life,damage,QSizeF(radius,radius),range),
	  radius(radius), color(c), fill(fill)
{

}

CircleEnemy::CircleEnemy(int life, int damage, const float radius,
						 QColor c, bool fill,
						 const PositionRange &range,
						 const QPointF &position)
	: CircleEnemy(life,damage, radius, c, fill, range)
{
	moveTo(position);
}

CircleEnemy::CircleEnemy(int life, int damage, const float radius,
						 QImage sprite, const PositionRange &range)
	: Enemy(life,damage,QSizeF(radius,radius),range),
	  radius(radius), sprite(sprite.scaledToWidth(radius*2))
{

}

CircleEnemy::CircleEnemy(int life, int damage, const float radius,
						 QImage sprite, const PositionRange &range,
						 const QPointF &position)
	: CircleEnemy(life, damage, radius, sprite, range)
{
	moveTo(position);
}

void CircleEnemy::paint(QPainter &context) const
{
	if(sprite.isNull()){
		if(fill)
			context.setBrush(color);
		else
			context.setBrush(Space::noBrush);
		context.setPen(color);
		context.drawEllipse(Space::origin,radius,radius);
	}
	else{
		context.drawImage(0,0,sprite);
	}
	if(isDead()){
		drawExplosion(context, radius*0.5, deadState);
	}

}

ColliderObject *CircleEnemy::collider() const
{
	return new ColliderCircle(radius,Space::origin);
}

void CircleEnemy::animate()
{
	Enemy::animate();
	if(isDead())
		deadState += 0.055f;
}
