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
private:
	QPointF _center;
	float _radius;
	ColliderType _type;
public:
	ColliderObject(ColliderType type, float radius, const QPointF& center);
	virtual bool isNearTo(const ColliderObject& object) const;
	virtual bool collisionWithObject(const ColliderObject& object) const = 0;
	virtual bool collisionWithPoint(const QPointF& A) const = 0;
	virtual int collisionWithSegment(const QPointF& A, const QPointF& B) const = 0;
	virtual void translate(float tx, float ty);
	ColliderObject* lightTranslate(float tx, float ty);

	QPointF center() const;
	float radius() const;
	ColliderType type() const;
};

class ColliderCircle : public ColliderObject{
public:
	ColliderCircle(const float radius, const QPointF center);
	// ColliderObject interface
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
};

class ColliderRect : public ColliderObject{
	QRectF _bounds;
public:
	ColliderRect(const QRectF& rect);

	// ColliderObject interface
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
	void translate(float tx, float ty);
	const QRectF& bounds() const;
};

class ColliderLine : public ColliderObject{
	QLineF _line;
public:
	ColliderLine(const QLineF& line);
	// ColliderObject interface
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
	void translate(float tx, float ty);
	const QLineF& line() const;
};

#endif // COLLIDEROBJECT_H
