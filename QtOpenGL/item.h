#ifndef ITEM_H
#define ITEM_H

#include "mapelement.h"

class Item : public MapElement
{
	bool dead;
public:
	Item(QList<Object3D*> models, Collider* collider);

	void actionOnCollision(Player &p);
	bool isDead();
	bool blockPath() const;
};

class DuffItem : public Item{
public:
	DuffItem(QList<Object3D*> models, Collider* collider);
	void actionOnCollision(Player &p);
};

#endif // ITEM_H
