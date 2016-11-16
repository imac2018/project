#include "space.h"

Space::Space()
{

}

void Space::addObject(SpaceObject * object)
{
	pendingItems.append(object);

}

void Space::paint(QPainter &context) const
{
	context.fillRect(context.viewport(),QColor(0,0,0));
	context.setPen(QColor(255,255,255));
	foreach (SpaceObject* o, items) {
		o->paint(context);
	}
}

void Space::animate()
{
	QMutableListIterator<SpaceObject*> i(items);
	QList<SpaceObject*>::iterator i2 = items.begin(), j;
	SpaceObject* o, *o2;

	while(i.hasNext()){
		o = i.next();
		o->animate();

		j = QList<SpaceObject*>::iterator(i2);
		j++;
		while(j!=items.end()){
			o2 = (*j);
			if(o2->collider()->collisionWithObject(*o->collider())){
				o->handleCollision(o2);
				o2->handleCollision(o);
			}
			j++;
		}

		if(o->isDead()){
			i.remove();
		}
		i2++;
	}
	while(!pendingItems.empty()){
		SpaceObject* object = pendingItems.dequeue();
		items.append(object);
		object->setWorld(this);
	}
}

void SpaceObject::setWorld(Space *world)
{
	this->world = world;
}
