#include "colliderobject.h"
#include <QVector2D>
#include <QDebug>
#include <QtMath>

inline bool pointOnTheRightOf(const QPointF &p, const QPointF &A, const QPointF &B)
{
	QVector2D AB(B - A), AP(p - A);
	return  AB.y() * AP.x() - AB.x() * AP.y() < 0; // inversed because the interface y axe go to bottom
}

inline int segmentOnTheRightOf(const QPointF &A, const QPointF &B,
							   const QPointF &C, const QPointF &D){
	if(pointOnTheRightOf(A,C,D) &&	pointOnTheRightOf(B,C,D))
		return 2;
	else
		return 0;
}

int intersectionBetweenSegment(const QPointF &A, const QPointF &B,
												const QPointF &C, const QPointF &D)
{
	QVector2D AB(B - A), CD(D - C);
	double denom = AB.x() * CD.y() - AB.y() * CD.x();
	if (!denom){
		return segmentOnTheRightOf(A,B,C,D);
	}
	float CAx = A.x()-C.x();
	float AC2y = A.y()+C.y();
	float ACy = C.y()-A.y();

	float t = - (CD.y()*CAx-CD.x()*AC2y) / denom;
	t = - (A.x() * CD.y() - C.x() * CD.y() - CD.x() * A.y() + CD.x() * C.y()) / denom;
	if (t<0 || t>1) {
		return segmentOnTheRightOf(A,B,C,D);
	}
	t = - (AB.x()*ACy+AB.y()*CAx) / denom;
	if(t>=0 && t<=1){
		return 1;
	}
	else{
		return segmentOnTheRightOf(A,B,C,D);
	}

}

ColliderObject::ColliderType ColliderObject::type() const
{
	return _type;
}

ColliderObject::ColliderObject(ColliderObject::ColliderType type, float radius, const QPointF &center)
	: _type(type), _radius(radius), _center(center)
{}

bool ColliderObject::isNearTo(const ColliderObject &object) const
{
	float r = _radius + object._radius;
	QPointF d = _center - object._center;
	return d.x() * d.x() + d.y() * d.y() < r * r;
}

void ColliderObject::translate(float tx, float ty)
{
	_center += QPointF(tx, ty);
}

ColliderObject *ColliderObject::lightTranslate(float tx, float ty)
{
	return new ColliderCircle(_radius, _center + QPointF(tx,ty));
}

QPointF ColliderObject::center() const
{
	return _center;
}

float ColliderObject::radius() const
{
	return _radius;
}

ColliderCircle::ColliderCircle(const float radius, const QPointF center) :
	ColliderObject(Circle, radius, center)
{}

bool ColliderCircle::collisionWithObject(const ColliderObject &object) const
{
	const ColliderCircle* c = dynamic_cast<const ColliderCircle*>(&object);
	float r;
	int ret1, ret2;
	QLineF line;

	switch(object.type()){
	case Circle:
		if(c != NULL){
			r = c->radius() + radius();
			return QVector2D(c->center() - center()).lengthSquared() < r * r;
		}
		return false;
	case Rect:
		if(c != NULL){
			const QRectF& bounds = (dynamic_cast<const ColliderRect*>(&object))->bounds();
			ret1 = ret2 = collisionWithSegment(bounds.topLeft(),bounds.topRight());
			if(ret1 == 1)
				return true;
			ret1 = collisionWithSegment(bounds.topRight(),bounds.bottomRight());
			if(ret1 == 1)
				return true;
			ret2 &= ret1;
			ret1 = collisionWithSegment(bounds.bottomRight(),bounds.bottomLeft());
			if(ret1 == 1)
				return true;
			ret2 &= ret1;
			ret1 = collisionWithSegment(bounds.bottomLeft(),bounds.topLeft());
			return (ret1==1) || ((ret1&ret2)==2);
		}
		break;
	case Line:
		line = (dynamic_cast<const ColliderLine*>(&object))->line();
		return collisionWithSegment(line.p1(),line.p2())==1;
	default:
		break;
	}
	return false;
}

bool ColliderCircle::collisionWithPoint(const QPointF &A) const
{
	QVector2D AC(A - center());
	return AC.lengthSquared() < radius() * radius();
}

int ColliderCircle::collisionWithSegment(const QPointF &A, const QPointF &B) const
{
	if(collisionWithPoint(A) || collisionWithPoint(B))
		return 1;
	QVector2D AB(B - A);
	QVector2D CA(A - center());
	float scalAB = AB.lengthSquared();
	float scalABCA = QVector2D::dotProduct(AB,CA);
	float scalOA = QPointF::dotProduct(A,A);
	float scalOC = QPointF::dotProduct(center(),center());
	float scalOCOA = QPointF::dotProduct(center(),A);

	float a = scalAB;
	float b = scalABCA * 2.f;
	float c = scalOA + scalOC - 2.f*(scalOCOA) - radius()*radius();
	float delta = b*b - 4.f*a*c;
	if(delta>EPSILON){
		float t1 = (-b + sqrtf(delta))/(2.f*a);
		float t2 = (-b - sqrtf(delta))/(2.f*a);
		if(t1>t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		if((t1 <= 0.f && t2 >=1.f) || (t2>=0.f && t2 <=1.f) || (t1 >=0.f && t1<=1.f))
		{
			return 1;
		}
	}
	else if(isZero(delta)){
		float t = (-b)/(2.f*a);
		if(t>=0.f && t<=1.f){
			return 1;
		}
	}
	if(pointOnTheRightOf(center(),A,B))
		return 2;
	return 0;
}


ColliderRect::ColliderRect(const QRectF &rect)
	: ColliderObject(Rect, qMax(rect.width(),rect.height()), rect.center()), _bounds(rect)
{}

bool ColliderRect::collisionWithObject(const ColliderObject &object) const
{
	const ColliderCircle* c = dynamic_cast<const ColliderCircle*>(&object);
	float r;
	int ret1, ret2;
	QLineF line;

	switch(object.type()){
	case Circle:
		if(c != NULL){
			ret1 = ret2 = c->collisionWithSegment(_bounds.topLeft(),_bounds.topRight());
			if(ret1 == 1)
				return true;
			ret1 = c->collisionWithSegment(_bounds.topRight(),_bounds.bottomRight());
			if(ret1 == 1)
				return true;
			ret2 &= ret1;
			ret1 = c->collisionWithSegment(_bounds.bottomRight(),_bounds.bottomLeft());
			if(ret1 == 1)
				return true;
			ret2 &= ret1;
			ret1 = c->collisionWithSegment(_bounds.bottomLeft(),_bounds.topLeft());
			return (ret1==1) || ((ret1&ret2)==2);
		}
		break;
	case Rect:
		if(c != NULL){
			const QRectF& bounds = dynamic_cast<const ColliderRect*>(&object)->bounds();
			ret1 = ret2 = collisionWithSegment(bounds.topLeft(),bounds.topRight());
			if(ret1 == 1)
				return true;
			ret1 = collisionWithSegment(bounds.topRight(),bounds.bottomRight());
			if(ret1 == 1)
				return true;
			ret2 &= ret1;
			r = collisionWithSegment(bounds.bottomRight(),bounds.bottomLeft());
			if(ret1 == 1)
				return true;
			ret2 &= ret1;
			ret1 = collisionWithSegment(bounds.bottomLeft(),bounds.topLeft());
			return (ret1==1) || ((ret1&ret2)==2);
		}
		break;
	case Line:
		line = dynamic_cast<const ColliderLine*>(&object)->line();
		return collisionWithSegment(line.p1(),line.p2()) > 0;
	default:
		break;
	}
	return false;
}

bool ColliderRect::collisionWithPoint(const QPointF &A) const
{
	if(! pointOnTheRightOf(A,_bounds.topLeft(),_bounds.topRight()))
		return false;
	if(! pointOnTheRightOf(A,_bounds.topRight(),_bounds.bottomRight()))
		return false;
	if(! pointOnTheRightOf(A,_bounds.bottomRight(),_bounds.bottomLeft()))
		return false;
	return pointOnTheRightOf(A,_bounds.bottomLeft(),_bounds.topLeft());
}

int ColliderRect::collisionWithSegment(const QPointF &A, const QPointF &B) const
{
	int ret1, ret2;
	ret1 = ret2 = intersectionBetweenSegment(A,B,_bounds.topLeft(),_bounds.topRight());
	if(ret1 == 1)
		return 1;
	ret1 = intersectionBetweenSegment(A,B,_bounds.topRight(),_bounds.bottomRight());
	if(ret1 == 1)
		return 1;
	ret2 &= ret1;
	ret1 = intersectionBetweenSegment(A,B,_bounds.bottomRight(),_bounds.bottomLeft());
	if(ret1 == 1)
		return 1;
	ret2 &= ret1;
	ret1 = intersectionBetweenSegment(A,B,_bounds.bottomLeft(),_bounds.topLeft());
	if(ret1==1)
		return ret1;
	return ret2 & ret1;
}

void ColliderRect::translate(float tx, float ty)
{
	ColliderObject::translate(tx, ty);
	_bounds.translate(tx, ty);
}

const QRectF &ColliderRect::bounds() const
{
	return _bounds;
}



ColliderLine::ColliderLine(const QLineF &line) : ColliderObject(Line,line.length()/2,line.pointAt(0.5)),
	_line(line)
{}

bool ColliderLine::collisionWithObject(const ColliderObject &object) const
{
	return (&object)->collisionWithSegment(_line.p1(),_line.p2())==1;
}

bool ColliderLine::collisionWithPoint(const QPointF &P) const
{
	QVector2D AP(P-_line.p1());
	QVector2D AB(_line.dx(), _line.dy());

	if(AP.x()*AB.x() - AP.y()*AB.x() == 0){
		float scal = QVector2D::dotProduct(AP, AB);
		return scal >= 0 && scal <= AB.lengthSquared();
	}
	return 0;
}

int ColliderLine::collisionWithSegment(const QPointF &A, const QPointF &B) const
{
	return intersectionBetweenSegment(_line.p1(),_line.p2(),A,B);
}

void ColliderLine::translate(float tx, float ty)
{
	ColliderObject::translate(tx, ty);
	_line.translate(QPointF(tx, ty));
}

const QLineF &ColliderLine::line() const
{
	return _line;
}


