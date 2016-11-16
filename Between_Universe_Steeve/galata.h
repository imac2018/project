#ifndef GALATA_H
#define GALATA_H

#include "mode.h"
#include "renderer.h"
#include "space.h"
#include "enemy.h"

class Player;
class GalataRenderer;
class Ship;

class Galata : public Mode
{
	friend class GalataRenderer;
private:
	class GalataRenderer : public Renderer{
		Galata* parent;
	public:
		GalataRenderer(Galata* parent);
	protected:
		virtual void paint(QPainter& context) const;
	};

	Player* p1;
	Space space;
	QPointF _initialShipPosition;

public:
	Galata(Player* p1);
	void setPlayer(Player* p1);
	virtual bool inputHandle(QInputEvent *event);
	QPointF initialShipPosition();
	void addShip(Ship* ship);
	void addEnemy(Enemy* e);

public slots:
	virtual bool update();
};

#endif // GALATA_H
