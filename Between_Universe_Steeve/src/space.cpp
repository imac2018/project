#include "space.h"
#include "weapon.h"

const QBrush Space::noBrush = QBrush(QColor(0,0,0,0));
const QPen Space::noPen = QPen(QColor(0,0,0,0));
const QPointF Space::origin = QPointF(0,0);

void Space::setPosition(SpaceObject &object)
{
	if(blockedItems.find(&object) != blockedItems.end()){
		QPointF requested = object.requestedPosition();
		QPointF actual(object.requestedPosition());
		if (requested.x() + object.size().width() > viewX + viewSize.width())
			actual.setX(viewX + viewSize.width() - 50);
		else if (requested.x() < viewX)
			actual.setX(viewX);
		if (requested.y() + object.size().height() > viewSize.height())
			actual.setY(viewSize.height() -  object.size().height());
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
	QPointF translate1, translate2, translateView(-viewX,0);
	while(i!=items.end())
	{
		o2 = (*i);
		if(o2 != (&object) && !o2->isDead()){
			co1 = object.collider();
			co2 = o2->collider();
			translate1 = translateView + object.actualPosition();
			translate2 = translateView + o2->actualPosition();
			if(co1->lightTranslate(translate1)
					->isNearTo(*(co2->lightTranslate(translate2))))
			{
				co1->translate(translate1);
				co2->translate(translate2);
				if(co1->collisionWithObject(*co2)){
					object.handleCollision(o2);
					o2->handleCollision(&object);
				}
			}
			delete co1,co2;
		}
		i++;
	}
}

Space::Space() : Space(QImage(1500,700,QImage::Format_ARGB32),
					   0,1000,700,1)
{}

Space::Space(QImage background, float viewX, float viewWidth,
			 float viewHeight, float xEnd, float velocity)
	: viewX(viewX), viewXEnd(xEnd), viewSize(viewWidth, viewHeight), scale(1),
	  velocity(velocity), background(background)
{
}

Space::~Space()
{
	QMutableListIterator<SpaceObject*> removerI(items);
	while(removerI.hasNext())
	{
		delete removerI.next();
		removerI.remove();
	}
}

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
	context.setPen(QColor(255,255,255));;

	int x1 = (int)(viewX) % (int)(background.width());
	int x2 = x1 + viewSize.width();
	int width, width2;
	if(x2 > background.width()){
		width = background.width() - x1;
		context.drawImage(QRect(0,viewSize.height()-background.height(),
								width,background.height()),
						  background,QRect(QPoint(x1,0),
										   QPoint(background.width(),background.height())));
		width2 = x2 - background.width();
		context.drawImage(QRect(width-1,viewSize.height()-background.height(),
								width2,background.height()),
						  background,QRect(QPoint(0,0),
										   QPoint(width2,background.height())));

	}
	else{
		width =x2 - x1;
		context.drawImage(QRect(0,viewSize.height()-background.height(),
								viewSize.width(),background.height()),
						  background,QRect(QPoint(x1,0),
										   QPoint(x2,background.height())));

	}
	context.translate(-viewX,0);
	x2 = viewX + viewSize.width();
	foreach (SpaceObject* o, items) {
		if(o->actualPosition().x()>=viewX-5 &&
				o->actualPosition().x()+o->size().width()<=x2+5){
			context.save();
			context.translate(o->_actualPosition.x(), o->_actualPosition.y());
			o->paint(context);
			context.restore();
		}
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
		if(o->isDefinitelyDead()){
			o->handleDeath();
			removerI.remove();
			delete o;
		}
		else if(!o->isInView(viewX, viewX + viewSize.width())){
			removerI.remove();
		}
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

void Space::resizeHandler(float windowWidth, float windowHeight)
{
	this->scale = windowWidth / this->viewSize.width();
	if(this->viewSize.height()*scale>windowHeight){
		this->scale = windowHeight / this->viewSize.height();
	}
}

QRectF Space::viewBound() const
{
	return QRectF(QPointF(viewX,0), viewSize);
}

bool Space::isEnded() const
{
	return viewX > viewXEnd;
}

void Space::applyTransform(QPainter &context, float windowWidth, float windowHeight)
{
	resizeHandler(windowWidth,windowHeight);
	context.translate(context.window().width()*0.5-viewSize.width()*0.5*scale,
					  context.window().height()*0.5-viewSize.height()*0.5*scale);
	context.scale(scale, scale);

}

void SpaceObject::setActualPosition(QPointF pos)
{
	this->_actualPosition = pos;
	this->_updated = true;
}

QSizeF SpaceObject::size() const
{
	return _size;
}

SpaceObject::SpaceObject(QSizeF size)
	: _updated(true), _actualPosition(0,0), _size(size) {}

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

void SpaceObject::handleDeath()
{}

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
	context.setPen(Space::noPen);
	context.setOpacity(1-state*0.5);
	context.drawEllipse(Space::origin,radius*0.1+radius*state*0.7,
						radius*0.1+radius*state*0.7);
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

B_TranslateAlternate::B_TranslateAlternate(QPair<float, float> range,
												QPointF translation)
	: B_Translate(translation), range(range), state(range.first), inc(1)
{}

B_TranslateAlternate::B_TranslateAlternate(float tstart, float tend, QPointF translation)
	: B_Translate(translation), range(tstart,tend), state(tstart), inc(1)
{
}

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
	cosInc(cosf(phase)),
	sinInc(sin(phase)),
	cosT(cosf(t)), sinT(sinf(t)),
	coefC(coefCos), coefS(coefSin),
	module(module)
{}

QPointF B_sinusoidal::move(QPointF initial){
	float cosT = this->cosT*cosInc - this->sinT*sinInc;
	float sinT = this->cosT*sinInc + this->sinT*cosInc;
	QPointF inc(-1, ((cosT - this->cosT) * coefC+
					  (sinT - this->sinT) * coefS) * module);
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

