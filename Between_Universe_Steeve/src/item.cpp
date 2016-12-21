#include "item.h"
#include "colliderobject.h"
#include "ship.h"
#include <QApplication>


Item::Item(int life, QPointF position, QSizeF size)
	: Item(life, position, size,
		   QPair<QPointF, QPointF>(QPointF(-1,-1),QPointF(-1,-1)))
{

}

Item::Item(int life, QPointF position, QSizeF size,
		   QPair<QPointF, QPointF> range)
	: SpaceObject(size), life(life), range(range)
{
	moveTo(position);
}

void Item::animate()
{
	life --;
}

void Item::setRange(QPair<QPointF, QPointF> range)
{
	this->range = range;
}

bool Item::isDead() const
{
	return life <=0;
}

bool Item::isInView(float x1, float x2) const
{
	if(range.first.x()<0)
		return true;
	else
		return range.first.x() < x2 && range.second.x() > x1;
}

I_LevelUp::I_LevelUp(int life, QPointF position)
	: Item(life, position, QSizeF(0,0))
{
	QFont f = QApplication::font();
	f.setPointSize(20);
	QFontMetricsF fm(f);
	_size = fm.size(Qt::TextSingleLine,"UP");
}

I_LevelUp::I_LevelUp(int life, QPointF position,
					 PositionRange range)
	: Item(life,position, QSizeF(0,0), range)
{
	QFont f = QApplication::font();
	f.setPointSize(20);
	QFontMetricsF fm(f);
	_size = fm.size(Qt::TextSingleLine,"UP") * 1.5f;
}

void I_LevelUp::paint(QPainter &context) const
{
	context.setPen(QColor(215,220,60));
	QFont f = context.font();
	f.setPointSize(20);
	f.setBold(true);
	context.setFont(f);
	QFontMetricsF fm(f);
	context.drawText(QRectF(Space::origin, _size),
					 QString("UP"));
}

ColliderObject *I_LevelUp::collider() const
{
	return new ColliderRect(QRectF(Space::origin, _size));
}

void I_LevelUp::animate()
{
	Item::animate();
}

void I_LevelUp::handleCollision(SpaceObject *o)
{
	Ship* s = dynamic_cast<Ship*>(o);
	if(s){
		s->levelUp();
		this->life = 0;
	}
}

Bonus::Bonus(Ship &ship, int delay) : ship(ship), delay(delay)
{}

bool Bonus::isOver() const
{
	return delay <= 0;
}

void Bonus::animate()
{
	delay--;
}

DefenseBonus::DefenseBonus(Ship &ship, int delay) : Bonus(ship,delay)
{}

void DefenseBonus::modifyShip() const
{
	Bonus::addShipDefense(ship);
}

void DefenseBonus::unmodifyShip() const
{
	Bonus::removeShipDefense(ship);
}

void DefenseBonus::paint(QPainter &context) const
{
	context.setPen(QPen(QColor(120,20,160),3));
	context.setBrush(QColor(180,120,210,50));
	float r = std::max<float>(ship.size().width(), ship.size().height());
	r *= 0.5;
	context.drawEllipse(QPointF(ship.size().width()*0.5,
								ship.size().height()*0.5), r,r);
}

void DefenseBonus::handleCollision()
{
	this->delay = 0;
}

I_Shield::I_Shield(int life, QPointF position)
	: Item(life,position,QSizeF(15,15))
{

}

I_Shield::I_Shield(int life, QPointF position,
				   PositionRange range) :
	Item(life,position, QSizeF(15,15),range)
{}

void I_Shield::paint(QPainter &context) const
{
	context.setPen(QPen(QColor(120,20,160),2));
	context.setBrush(QColor(180,120,210,50));

	context.drawEllipse(Space::origin,
						_size.width(),_size.height());
}

ColliderObject *I_Shield::collider() const
{
	return new ColliderCircle(_size.width(), Space::origin);
}

void I_Shield::animate()
{
	Item::animate();
	moveTo(actualPosition() + QPointF(0,2));
}

void I_Shield::handleCollision(SpaceObject *o)
{
	Ship* s = dynamic_cast<Ship*>(o);
	if(s){
		s->addBonus(new DefenseBonus(*s,360));
		this->life = 0;
	}
}
