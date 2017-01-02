#include "mapelement.h"
#include "tools.h"

MapElement::MapElement(QList<Object3D *> models, Collider *collider)
	: collider(collider)
{
	foreach(Object3D* model, models){
		_objects.append(new Object3D(*model));
	}

	unhighlight();
}

bool MapElement::intersectRay(QVector3D source, QVector3D direction)
{
	return collider->intersectRay(transformM, source,direction);
}

void MapElement::highlight()
{
	foreach (Object3D* o, _objects) {
		o->ambiantColor = Qt::white;
		o->diffuseColor = QColor(255,255,200);
		o->specularColor = QColor(200,200,255);
	}
}

void MapElement::unhighlight()
{
	foreach (Object3D* o, _objects) {
		o->ambiantColor = Qt::lightGray;
		o->diffuseColor = Qt::gray;
		o->specularColor = Qt::black;
	}
}



void MapElement::update(Level &)
{
	if(behaviors.length()){
		QList<Behavior*>::Iterator i;
		for(i = behaviors.begin(); i != behaviors.end();i++){
			transformM = (*i)->move(transformM);
			foreach (Object3D* o, _objects) {
				o->transformM = (*i)->move(o->transformM);
			}
		}
	}
}

void MapElement::actionOnClick(Player &p)
{
	actionOnCollision(p);
}

float MapElement::x() const
{
	return transformM(0,3);
}

float MapElement::y() const
{
	return transformM(1,3);
}

float MapElement::z() const
{
	return transformM(2,3);
}

QVector3D MapElement::position() const
{
	return transformM.column(3).toVector3D();
}

void MapElement::transform(const QMatrix4x4& transformM)
{
	foreach(Object3D* o, _objects){
		o->transformM = transformM * o->transformM;
	}
	this->transformM = transformM * this->transformM;
}

void MapElement::transformRight(const QMatrix4x4 &transformM)
{
	foreach(Object3D* o, _objects){
		o->transformM = o->transformM * transformM;
	}
	this->transformM = this->transformM * transformM;
}

void MapElement::resetPosition()
{
	foreach(Object3D* o, _objects){
		o->transformM.setColumn(3,QVector4D(0,0,0,1));
	}
	this->transformM.setColumn(3,QVector4D(0,0,0,1));
}

void MapElement::resetTransformation()
{
	foreach(Object3D* o, _objects){
		o->transformM.setToIdentity();
	}
	this->transformM.setToIdentity();
}

QList<Object3D *> MapElement::objects() const
{
	return _objects;
}


Mat4Behavior::Mat4Behavior()
{}

Mat4Behavior::Mat4Behavior(QMatrix4x4 transform)
	: transform(transform)
{}

QMatrix4x4 Mat4Behavior::move(const QMatrix4x4& initial)
{
	return initial * transform;
}

AlternateBehavior::AlternateBehavior(QPair<int, int> range, Mat4Behavior *mainBehavior)
	: range(range), state(range.first), inc(1), mainBehavior(mainBehavior)
{}

AlternateBehavior::AlternateBehavior(int tstart, int tend, Mat4Behavior *mainBehavior)
	: range(tstart, tend), state(range.first), inc(1), mainBehavior(mainBehavior)
{}

QMatrix4x4 AlternateBehavior::move(const QMatrix4x4& initial)
{
	state += inc;
	if(state<range.first || state>range.second){
		inc *= -1;
		mainBehavior->transform = mainBehavior->transform.inverted();
	}
	return mainBehavior->move(initial);
}

ColliderRect::ColliderRect(QVector3D u, QVector3D v, QVector3D A)
	: u(u,0), v(v,0), A(A,1)
{}

QPair<QVector3D, QVector3D> findMinMax(QVector3D* vectors[3]){
	QVector3D min = *vectors[0];
	QVector3D max = *vectors[0];
	for(int i=1;i<3;i++){
		if(vectors[i]->x()<min.x())
			min.setX(vectors[i]->x());
		else if(vectors[i]->x()>max.x())
			max.setX(vectors[i]->x());

		if(vectors[i]->y()<min.y())
			min.setY(vectors[i]->y());
		else if(vectors[i]->y()>max.y())
			max.setY(vectors[i]->y());

		if(vectors[i]->z()<min.z())
			min.setZ(vectors[i]->z());
		else if(vectors[i]->z()>max.z())
			max.setZ(vectors[i]->z());
	}
	return QPair<QVector3D, QVector3D>(min,max);
}

bool ColliderRect::intersectRay(QMatrix4x4 transform, QVector3D source, QVector3D direction) const
{
	QVector3D u = (transform * this->u).toVector3D(),
			v = (transform * this->v).toVector3D(),
			A = (transform * this->A).toVector3D();
	QVector3D n = QVector3D::crossProduct(u, v);
	float denom = QVector3D::dotProduct(n,direction);
	if(isZero(denom))
		return false;
	float t0 = -(QVector3D::dotProduct(n,source)
					-QVector3D::dotProduct(n,A)) / denom;
	if(t0<=0)
		return false;
	QVector3D p = source + direction * t0;

	QVector3D B = A + u, C = A + v;
	QVector3D* vectors[3] = {&A,&B,&C};

	QPair<QVector3D, QVector3D> min_max = findMinMax(vectors);
	if(!isZero(min_max.first.x() - min_max.second.x()))
		if (!(p.x() >= min_max.first.x() && p.x() <= min_max.second.x()))
			return false;
	if(!isZero(min_max.first.y() - min_max.second.y()))
		if (!(p.y() >= min_max.first.y() && p.y() <= min_max.second.y()))
			return false;
	if(!isZero(min_max.first.z() - min_max.second.z()))
		if (!(p.z() >= min_max.first.z() && p.z() <= min_max.second.z()))
			return false;
	return true;
}

ColliderCube::ColliderCube(QVector3D o, QVector3D u, QVector3D v, QVector3D w)
	: rect1(u+w, v, o), rect2(u-w,v,o+w)
{}

bool ColliderCube::intersectRay(QMatrix4x4 transform, QVector3D source, QVector3D direction) const
{
	if(rect1.intersectRay(transform, source,direction))
		return true;
	return rect2.intersectRay(transform, source, direction);
}
