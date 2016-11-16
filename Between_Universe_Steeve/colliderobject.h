#ifndef COLLIDEROBJECT_H
#define COLLIDEROBJECT_H

#define EPSILON 0.0005
#define isZero(number) (number > -EPSILON && number < EPSILON)

#include <QPointF>
#include <QRectF>
#include <QLineF>

class ColliderObject{

protected:

	enum ColliderType{
		Circle,
		Rect,
		Line,
		Wall,
		Ray,
		Polygone,
		NbColliderType
	};
	ColliderType type;
	ColliderType getType(const ColliderObject& o) const;
public:
	ColliderObject(ColliderType type);
	virtual bool collisionWithObject(const ColliderObject& object) const = 0;
	virtual bool collisionWithPoint(const QPointF& A) const = 0;
	virtual int collisionWithSegment(const QPointF& A, const QPointF& B) const = 0;
};

class ColliderCircle : public ColliderObject{
protected:
	const float* radius;
	const QPointF* center;
public:
	ColliderCircle(const float* radius, const QPointF* center);
	// ColliderObject interface
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
	float getRadius() const;
	const QPointF& getCenter() const;
};

class ColliderRect : public ColliderCircle{
	const QRectF* _bounds;
public:
	ColliderRect(const float *radius, const QPointF* center, const QRectF* rect);

	// ColliderObject interface
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
	const QRectF& bounds() const;
};

class ColliderLine : public ColliderObject{
	const QLineF* _line;
public:
	ColliderLine(const QLineF* line);
	// ColliderObject interface
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
	const QLineF& line() const;
};

#endif // COLLIDEROBJECT_H
