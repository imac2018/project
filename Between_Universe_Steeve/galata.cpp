#include "galata.h"
#include "player.h"
#include "math.h"

Galata::Galata(Player *p1) : _initialShipPosition(5,60)
{
	_renderer = new GalataRenderer(this);
	addPlayer(p1);
}

void Galata::addPlayer(Player *p)
{
	this->players.append(p);
	p->addShipToGalata(*this);
}

bool Galata::inputHandle(QInputEvent* event)
{
	bool _return;
	foreach (Player* p, players) {
		_return |= p->inputHandle(event);
	}
	return _return;
}

bool Galata::update()
{
	_space.animate();
	return true;
}

QPointF Galata::initialShipPosition()
{
	return _initialShipPosition;
}

void Galata::addShip(Ship *ship)
{
	_space.addBlockedObject(ship);
	ship->moveTo(initialShipPosition());
	_space.addObject(ship);
}

void Galata::addEnemy(Enemy *e)
{
	_space.addObject(e);
}

const Space &Galata::space()
{
	return _space;
}

Galata::GalataRenderer::GalataRenderer(Galata *p) : parent(p)
{

}

void Galata::GalataRenderer::paint(QPainter &context) const
{
	context.save();
	parent->_space.paint(context);
	context.restore();
	context.setPen(QColor(20,240,135));
	QFont f = context.font();
	f.setPointSize(15);
	context.setFont(f);
	QFontMetrics fontMetrics(f);
	int x = 10, y1 = fontMetrics.height() * 1.5, y2 = y1 + fontMetrics.height()*1.2;
	foreach(Player* p, parent->players){
		context.drawText(x,y1, p->name());
		context.drawText(x,y2, QString::number(p->points()));
		x += std::max<int>(fontMetrics.width(p->name()), fontMetrics.width(QString::number(p->points()))) +40;
	}
}
