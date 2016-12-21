#ifndef COLLIDEROBJECT_H
#define COLLIDEROBJECT_H

#define EPSILON 0.0005
#define isZero(number) (number > -EPSILON && number < EPSILON)

#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QList>

/**
 * @brief Interface wich allow the test of collision between two object
 * Set a ColliderObject in a object to define its collision physic
 */
class ColliderObject{
protected:

	/**
	 * @brief This enum allow an object to know how to proceed the collision test with an another
	 */
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
	/**
	 * @brief ColliderObject
	 * @param type
	 * @param radius The radius is used to make a first approach of the collision,
	 * if the objects are to far each other, no need to proceed the remainder
	 * @param center The center is used to know approximately the position of the object.
	 * Like the radius, it is used to have a first approach of the collision
	 */
	ColliderObject(ColliderType type, float radius, const QPointF& center);
	virtual bool isNearTo(const ColliderObject& object) const;
	virtual bool collisionWithObject(const ColliderObject& object) const = 0;
	virtual bool collisionWithPoint(const QPointF& A) const = 0;
	virtual int collisionWithSegment(const QPointF& A, const QPointF& B) const = 0;
	virtual void translate(float tx, float ty);
	ColliderObject* lightTranslate(float tx, float ty) const;
	void translate(const QPointF& t);
	ColliderObject* lightTranslate(const QPointF& t) const;

	QPointF center() const;
	float radius() const;
	ColliderType type() const;
};

class ColliderCircle : public ColliderObject{
public:
	ColliderCircle(const float radius, const QPointF center);
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
};

class ColliderRect : public ColliderObject{
	QRectF _bounds;
public:
	ColliderRect(const QRectF& rect);
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
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
	void translate(float tx, float ty);
	const QLineF& line() const;
};

class ColliderPolygon : public ColliderObject{
	QList<QPointF> _points;
public:
	ColliderPolygon(float radius, QPointF center);
	ColliderPolygon(float radius, QPointF center, const QList<QPointF>& points);
	void addPoint(QPointF point);
	// ColliderObject interface
public:
	bool collisionWithObject(const ColliderObject &object) const;
	bool collisionWithPoint(const QPointF &A) const;
	int collisionWithSegment(const QPointF &A, const QPointF &B) const;
	void translate(float tx, float ty);
	const QList<QPointF>& points() const;
};

#endif // COLLIDEROBJECT_H
