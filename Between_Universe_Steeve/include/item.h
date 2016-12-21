#ifndef ITEMS_H
#define ITEMS_H

#include "space.h"

class Ship;

/**
 * @brief An Item is a SpaceObject which can modify the Ship features
 */
class Item : public SpaceObject
{
protected:
	int life;
	QPair<QPointF, QPointF> range;
public:
	Item(int life, QPointF position, QSizeF _size);
	Item(int life, QPointF position, QSizeF _size,
		 QPair<QPointF, QPointF> range);
	void animate();
	void setRange(QPair<QPointF, QPointF> range);
	bool isDead() const;
	bool isInView(float x1, float x2) const;
};

/**
 * @brief The Bonus define a temporal feature for a ship
 */
class Bonus{
protected:
	int delay;
	Ship& ship;
public:
	Bonus(Ship& ship, int delay);
	virtual void modifyShip() const = 0;
	virtual void unmodifyShip() const = 0;
	void addShipDefense(Ship& ship) const;
	void removeShipDefense(Ship& ship) const;
	/**
	 * @brief draw something more in the ship
	 */
	virtual void paint(QPainter& context) const = 0;
	/**
	 * @brief define what's happen if the ship has been hurt
	 */
	virtual void handleCollision() = 0;
	bool isOver() const;
	virtual void animate();
};

class DefenseBonus : public Bonus{
public:
	DefenseBonus(Ship& ship, int delay);
	virtual void modifyShip() const;
	virtual void unmodifyShip() const;
	virtual void paint(QPainter& context) const;
	virtual void handleCollision();
};


class I_LevelUp : public Item{
public:
	I_LevelUp(int life, QPointF position);
	I_LevelUp(int life, QPointF position, PositionRange range);
	void paint(QPainter &context) const;
	ColliderObject *collider() const;
	void animate();
	void handleCollision(SpaceObject *o);
};

class I_Shield : public Item{
public:
	I_Shield(int life, QPointF position);
	I_Shield(int life, QPointF position, PositionRange range);
	void paint(QPainter &context) const;
	ColliderObject *collider() const;
	void animate();
	void handleCollision(SpaceObject *o);
};

#endif // ITEMS_H

