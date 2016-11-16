#include "colliderobject.h"
#include <QVector2D>

inline bool pointOnTheRightOf(const QPointF &p, const QPointF &A, const QPointF &B)
{
	QVector2D AB(B - A), AP(p - A);
	return AB.x() * AP.y() - AB.y() * AP.x() < 0;
}

inline int segmentOnTheRightOf(const QPointF &A, const QPointF &B,
							   const QPointF &C, const QPointF &D){
	if(pointOnTheRightOf(A,C,D) &&
			pointOnTheRightOf(B,C,D))
		return 2;
	else
		return 0;
}

int intersectionBetweenSegment(const QPointF &A, const QPointF &B,
												const QPointF &C, const QPointF &D)
{
	QVector2D AB(B - A), CD(D - C);
	double denom = AB.x() * CD.y() - AB.y() - CD.x();
	if (!denom){
		return segmentOnTheRightOf(A,B,C,D);
	}

	float CAx = A.x()-C.x();
	float ACy = A.y()-A.y();

	float t = - (CD.y()*CAx+CD.x()*ACy) / denom;
	if (t<0 || t>1) {
		return segmentOnTheRightOf(A,B,C,D);
	}

	t = - (AB.x()*ACy+AB.y()*CAx) / denom;
	if(t<=0 && t>=1){
		return 1;
	}
	else{
		return segmentOnTheRightOf(A,B,C,D);
	}

}

ColliderObject::ColliderType ColliderObject::getType(const ColliderObject &o) const
{
	return o.type;
}

ColliderObject::ColliderObject(ColliderObject::ColliderType type)
	: type(type)
{}

ColliderCircle::ColliderCircle(const float *radius, const QPointF *center) :
	ColliderObject(Circle), radius(radius), center(center)
{}

bool ColliderCircle::collisionWithObject(const ColliderObject &object) const
{
	const ColliderCircle* c = dynamic_cast<const ColliderCircle*>(&object);
	float r;
	int ret1, ret2;
	QLineF line;

	switch(getType(object)){
	case Circle:
		if(c != NULL){
			r = (*c->radius) + (*radius);
			return QVector2D((*c->center) - (*center)).lengthSquared() < r * r;
		}
		return false;
	case Rect:
		if(c != NULL){
			r = (*c->radius) + (*radius);
			if(QVector2D(*(c->center) - *(center)).lengthSquared() < r * r){
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
	QVector2D AC(A - *center);
	return AC.lengthSquared() < (*radius) * (*radius);
}

int ColliderCircle::collisionWithSegment(const QPointF &A, const QPointF &B) const
{
	if(collisionWithPoint(A) || collisionWithPoint(B))
		return 1;
	QVector2D AB(B - A);
	QVector2D CA(A - *center);
	float scalAB = AB.lengthSquared();
	float scalABCA = QVector2D::dotProduct(AB,CA);
	float scalOA = QPointF::dotProduct(A,A);
	float scalOC = QPointF::dotProduct(*center,*center);
	float scalOCOA = QPointF::dotProduct(*center,A);

	float a = scalAB;
	float b = scalABCA * 2.f;
	float c = scalOA + scalOC - 2.f*(scalOCOA) - (*radius)*(*radius);
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
	if(pointOnTheRightOf(*center,A,B))
		return 2;
	return 0;
}

float ColliderCircle::getRadius() const
{
	return *radius;
}

const QPointF &ColliderCircle::getCenter() const
{
	return *center;
}


ColliderRect::ColliderRect(const float* radius, const QPointF *center, const QRectF *rect)
	: ColliderCircle(radius,center), _bounds(rect)
{
	this->type = Rect;
}

bool ColliderRect::collisionWithObject(const ColliderObject &object) const
{
	const ColliderCircle* c = dynamic_cast<const ColliderCircle*>(&object);
	float r;
	int ret1, ret2;
	QLineF line;

	switch(getType(object)){
	case Circle:
		if(c != NULL){
			r = c->getRadius() + getRadius();
			if(QVector2D(c->getCenter() - getCenter()).lengthSquared() < r * r){
				ret1 = ret2 = c->collisionWithSegment(_bounds->topLeft(),_bounds->topRight());
				if(ret1 == 1)
					return true;
				ret1 = c->collisionWithSegment(_bounds->topRight(),_bounds->bottomRight());
				if(ret1 == 1)
					return true;
				ret2 &= ret1;
				ret1 = c->collisionWithSegment(_bounds->bottomRight(),_bounds->bottomLeft());
				if(ret1 == 1)
					return true;
				ret2 &= ret1;
				ret1 = c->collisionWithSegment(_bounds->bottomLeft(),_bounds->topLeft());
				return (ret1==1) || ((ret1&ret2)==2);
			}
		}
		break;
	case Rect:
		if(c != NULL){
			r = c->getRadius() + getRadius();
			if(QVector2D(c->getCenter() - getCenter()).lengthSquared() < r * r){
				const QRectF& bounds = dynamic_cast<const ColliderRect*>(&object)->bounds();
				ret1 = ret2 = collisionWithSegment(bounds.topLeft(),bounds.topRight());
				if(r == 1)
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
		}
		break;
	case Line:
		line = dynamic_cast<const ColliderLine*>(&object)->line();
		return collisionWithSegment(line.p1(),line.p2())==1;
	default:
		break;
	}
	return false;
}

bool ColliderRect::collisionWithPoint(const QPointF &A) const
{
	if(! pointOnTheRightOf(A,_bounds->topLeft(),_bounds->topRight()))
		return false;
	if(! pointOnTheRightOf(A,_bounds->topRight(),_bounds->bottomRight()))
		return false;
	if(! pointOnTheRightOf(A,_bounds->bottomRight(),_bounds->bottomLeft()))
		return false;
	return pointOnTheRightOf(A,_bounds->bottomLeft(),_bounds->topLeft());
}

int ColliderRect::collisionWithSegment(const QPointF &A, const QPointF &B) const
{
	int ret1, ret2;
	ret1 = ret2 = intersectionBetweenSegment(A,B,_bounds->topLeft(),_bounds->topRight());
	if(ret1 == 1)
		return 1;
	ret1 = intersectionBetweenSegment(A,B,_bounds->topRight(),_bounds->bottomRight());
	if(ret1 == 1)
		return 1;
	ret2 &= ret1;
	ret1 = intersectionBetweenSegment(A,B,_bounds->bottomRight(),_bounds->bottomLeft());
	if(ret1 == 1)
		return 1;
	ret2 &= ret1;
	ret1 = intersectionBetweenSegment(A,B,_bounds->bottomLeft(),_bounds->topLeft());
	if((ret1==1))
		return 1;
	if(segmentOnTheRightOf(_bounds->topLeft(),_bounds->bottomLeft(),A,B))
		return 2;
	return 0;
}

const QRectF &ColliderRect::bounds() const
{
	return *_bounds;
}



ColliderLine::ColliderLine(const QLineF *line) : ColliderObject(Line), _line(line)
{}

bool ColliderLine::collisionWithObject(const ColliderObject &object) const
{
	const ColliderObject* c = dynamic_cast<const ColliderObject*>(&object);
	return c->collisionWithSegment(_line->p1(),_line->p2());
}

bool ColliderLine::collisionWithPoint(const QPointF &P) const
{
	QVector2D AP(P-_line->p1());
	QVector2D AB(_line->dx(), _line->dy());

	if(AP.x()*AB.x() - AP.y()*AB.x() == 0){
		float scal = QVector2D::dotProduct(AP, AB);
		return scal >= 0 && scal <= AB.lengthSquared();
	}
	return 0;
}

int ColliderLine::collisionWithSegment(const QPointF &A, const QPointF &B) const
{
	return intersectionBetweenSegment(_line->p1(),_line->p2(),A,B);
}

const QLineF &ColliderLine::line() const
{
	return *_line;
}


