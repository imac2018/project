#include "space.h"
#include <QDebug>
#include "weapon.h"

void Space::setPosition(SpaceObject &object)
{
	if(blockedItems.find(&object) != blockedItems.end()){
		QPointF requested = object.requestedPosition();
		QPointF actual(object.requestedPosition());
		if (requested.x() > viewX + viewSize.width() - 50)
			actual.setX(viewX + viewSize.width() - 50);
		else if (requested.x() < viewX)
			actual.setX(viewX);
		if (requested.y() > viewSize.height())
			actual.setY(viewSize.height());
		else if (requested.y() < 0)
			actual.setY(0);
		object.setActualPosition(actual);
	}
	else if(!object.updated()){
		object.setActualPosition(object.requestedPosition());
	}
	else
		return;
	if(object.isDead())
		return;
	QList<SpaceObject*>::iterator i = items.begin();
	SpaceObject* o2;
	ColliderObject* co1, *co2;
	while(i!=items.end()){
		o2 = (*i);
		if(o2 != (&object) && !o2->isDead()){
			co1 = object.collider();
			co2 = o2->collider();
			if(co1->lightTranslate(-viewX, 0)
					->isNearTo(*(co2->lightTranslate(-viewX,0)))){
				co1->translate(-viewX,0);
				co2->translate(-viewX,0);
				if(co1->collisionWithObject(*co2)){
					object.handleCollision(o2);
					o2->handleCollision(&object);
				}
			}
		}
		i++;
	}
}

Space::Space() : Space(0,1000,700,1)
{}

Space::Space(float viewX, float viewWidth, float viewHeight, float velocity)
	: viewX(viewX), viewSize(viewWidth, viewHeight), scale(1), velocity(velocity)
{}

void Space::addObject(SpaceObject * object)
{
	pendingItems.append(object);
	setPosition(*object);
}

void Space::addBlockedObject(SpaceObject *object)
{
	blockedItems.insert(object);
}

void Space::paint(QPainter &context) const
{
	context.fillRect(context.viewport(),QColor(0,0,0));
	context.setPen(QColor(255,255,255));;

	context.resetMatrix();
	context.resetTransform();
	context.scale(scale, scale);
	context.translate(-viewX,0);

	foreach (SpaceObject* o, items) {
		context.save();
		context.translate(o->_actualPosition.x(), o->_actualPosition.y());
		o->paint(context);
		context.restore();
	}
}

void Space::animate()
{
	QMutableListIterator<SpaceObject*> removerI(items);
	QList<SpaceObject*>::iterator i = items.begin();
	SpaceObject* o;

	for(;i!=items.end();i++){
		o = *i;
		o->animate();
		setPosition(*o);
	}

	while(removerI.hasNext()){
		o = removerI.next();
		if(o->isDefinitelyDead() || !o->isInView(viewX, viewX + viewSize.width()))
			removerI.remove();
	}

	removerI = QMutableListIterator<SpaceObject*>(pendingItems);
	while(removerI.hasNext()){
		o = removerI.next();
		if(o->isInView(viewX, viewX + viewSize.width())){
			items.append(o);
			setPosition(*o);
			o->setWorld(this);
			removerI.remove();
		}
	}
	viewX += velocity;
}

void Space::resizeHandler(float windowWidth, float)
{
	this->scale = windowWidth / this->viewSize.width();
}

QRectF Space::viewBound() const
{
	return QRectF(QPointF(viewX,0), viewSize);
}

void SpaceObject::setActualPosition(QPointF pos)
{
	this->_actualPosition = pos;
	this->_updated = true;
}

void SpaceObject::moveTo(const QPointF &p)
{
	this->_requestedPosition = p;
	this->_updated = false;
}

bool SpaceObject::updated()
{
	return _updated;
}

void SpaceObject::setWorld(Space *world)
{
	this->world = world;
}

bool SpaceObject::isDefinitelyDead() const
{
	return this->isDead();
}

QPointF SpaceObject::actualPosition() const
{
	return _actualPosition;
}

QPointF SpaceObject::requestedPosition() const
{
	return _requestedPosition;
}

void SpaceObject::drawExplosion(QPainter& context, float radius, float state)
{
	float x = state - 0.5;
	x *= x;
	x += 0.5;
	context.setBrush(QColor(x*125+60,x*125+60,40));
	context.setPen(NoPen);
	context.setOpacity(1-state*0.5);
	context.drawEllipse(QPointF(0,0),radius*0.1+radius*state*0.7,radius*0.1+radius*state*0.7);
	context.setOpacity(1-state);
	context.setPen(QColor(x*255,0,15));
	context.setBrush(QColor(x*255,0,15));
	radius *=state;
	context.drawEllipse(QPointF(radius,radius),5,5);
	context.drawEllipse(QPointF(-radius,radius),6,5);
	context.drawEllipse(QPointF(-radius,-radius),5,5);
	context.drawEllipse(QPointF(radius,-radius),6,5);
}

B_TurnAround::B_TurnAround(float angleSpeed, float distance, float initialAngle)
	: rotateSpeedX(cosf(angleSpeed)),
	  rotateSpeedY(sinf(angleSpeed)),
	  rotateValueX(cosf(initialAngle)),
	  rotateValueY(sinf(initialAngle)),
	  distance(distance)
{}

QPointF B_TurnAround::move(QPointF initial)
{
	float newRotateX = rotateValueX*rotateSpeedX - rotateValueY*rotateSpeedY;
	float newRotateY = rotateValueX*rotateSpeedY + rotateValueY*rotateSpeedX;
	QPointF inc((newRotateX - rotateValueX), (newRotateY - rotateValueY));
	rotateValueX = newRotateX;
	rotateValueY = newRotateY;
	inc *= distance;
	return initial + inc;
}

B_Translate::B_Translate(QPointF translation) : translation(translation)
{}

QPointF B_Translate::move(QPointF initial)
{
	return initial + translation;
}

B_TranslateAlternate::B_TranslateAlternate(QPair<float, float> range, QPointF translation) :
	B_Translate(translation), range(range), state(range.first), inc(1)
{}

QPointF B_TranslateAlternate::move(QPointF initial)
{
	state += inc;
	if(state<range.first || state>range.second){
		inc *= -1;
		translation *= -1;
	}
	return initial + translation;
}

B_Temporal::B_Temporal(Behavior *b, int t_start, int t_end)
	: behavior(b), t(0), t_start(t_start), t_end(t_end)
{}

QPointF B_Temporal::move(QPointF initial)
{
	if(t<t_end){
		if(t>t_start){
			return behavior->move(initial);
		}
		t++;
	}
	return initial;
}



B_sinusoidal::B_sinusoidal(float phase, float module,
						   float coefCos, float coefSin) :
	t(0),
	cosInc(cosf(phase)*coefCos*module),
	sinInc(sin(phase)*coefSin*module),
	cosT(coefCos * cosf(t)), sinT(coefSin * sinf(t))
{}

QPointF B_sinusoidal::move(QPointF initial){
	float cosT = this->cosT*cosInc - this->sinT*sinInc;
	float sinT = this->cosT*sinInc + this->sinT*cosInc;
	QPoint inc(cosT - this->cosT, sinT - this->sinT);
	this->cosT = cosT;
	this->sinT = sinT;
	return initial + inc;
}

B_TemporalCycle::B_TemporalCycle(Behavior *b, int t_start,
								 int t_end, int cycle)
	: B_Temporal(b,t_start, t_end), cycle(cycle)
{}

QPointF B_TemporalCycle::move(QPointF initial)
{
	t = t%cycle;
	return B_Temporal::move(initial);
}

B_String::B_String() : t(0), current(behaviors.begin())
{}

B_String::B_String(LimitedBehaviors behaviors)
	: t(0), behaviors(behaviors), current(this->behaviors.begin()){}

void B_String::addBehavior(QPair<Behavior *, int> behavior)
{
	if(current != behaviors.end())
		current = behaviors.insert(current, behavior);
	else{
		behaviors.append(behavior);
		current = behaviors.end();
	}
	if(current != behaviors.begin()){
		current--;
	}
}

void B_String::addBehavior(Behavior *behavior, int duration)
{
	if(current != behaviors.end()){
		current = behaviors.insert(current, QPair<Behavior *, int>(behavior, duration));
	}
	else{
		behaviors.append(QPair<Behavior *, int>(behavior, duration));
		current = behaviors.end();
	}
	if(current != behaviors.begin()){
		current--;
	}
}

QPointF B_String::move(QPointF initial)
{
	if(current != behaviors.end()){
		QPointF p = (*current).first->move(initial);
		if(t>(*current).second){
			current++;
			t = 0;
		}
		else
			t++;
		return p;
	}
	return initial;

}

B_StringCycle::B_StringCycle(int delayBetweenCycle) :
	delayBetweenCycle(delayBetweenCycle)
{}

B_StringCycle::B_StringCycle(LimitedBehaviors behaviors, int delayBetweenCycle)
	: B_String(behaviors), delayBetweenCycle(delayBetweenCycle)
{}

QPointF B_StringCycle::move(QPointF initial)
{
	QPointF p = B_String::move(initial);
	if(current == behaviors.end()){
		if(t>delayBetweenCycle)
		{
			t++;
		}
		else{
			current = behaviors.begin();
			t=0;
		}
	}
	return p;
}

