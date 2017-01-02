#include "item.h"


Item::Item(QList<Object3D*> models, Collider *collider)
	: MapElement(models, collider), dead(false)
{
	QMatrix4x4 transformer;
	transformer.rotate(2,0,1,0);
	behaviors.append(new Mat4Behavior(transformer));
	transformer.setToIdentity();
	transformer.translate(0,0.025f,0);
	behaviors.append(new AlternateBehavior(0,250,
										   new Mat4Behavior(transformer)));
}

void Item::actionOnCollision(Player &)
{
	dead = true;
}

bool Item::isDead()
{
	return dead;
}

bool Item::blockPath() const
{
	return false;
}

DuffItem::DuffItem(QList<Object3D *> models, Collider *collider)
	: Item(models, collider)
{
	//this->transformM.scale(3.f);
}

void DuffItem::actionOnCollision(Player &p)
{
	if(!isDead()){
		Item::actionOnCollision(p);
		p.incrementDuff();
	}
}


