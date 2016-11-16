#include "galata.h"
#include "player.h"

Galata::Galata(Player *p1) : _initialShipPosition(5,60)
{
	_renderer = new GalataRenderer(this);
	this->p1 = p1;
	p1->addShipToGalata(*this);
}

void Galata::setPlayer(Player *p1)
{
	this->p1 = p1;
	p1->addShipToGalata(*this);
}

bool Galata::inputHandle(QInputEvent* event)
{
	return p1->inputHandle(event);
}

bool Galata::update()
{
	space.animate();
	return true;
}

QPointF Galata::initialShipPosition()
{
	return _initialShipPosition;
}

void Galata::addShip(Ship *ship)
{
	space.addObject(ship);
	ship->position = initialShipPosition();
}

void Galata::addEnemy(Enemy *e)
{
	space.addObject(e);
}

Galata::GalataRenderer::GalataRenderer(Galata *p) : parent(p)
{

}

void Galata::GalataRenderer::paint(QPainter &context) const
{
	parent->space.paint(context);
}
