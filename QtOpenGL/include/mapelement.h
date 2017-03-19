#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include <QVector3D>
#include "object3d.h"
#include "level.h"

class Behavior{
public:
	virtual QMatrix4x4 move(const QMatrix4x4& initial) = 0;
};

class Mat4Behavior : public Behavior{
public:
	QMatrix4x4 transform;
	Mat4Behavior();
	Mat4Behavior(QMatrix4x4 transform);
	virtual QMatrix4x4 move(const QMatrix4x4& initial);
};

class AlternateBehavior : public Behavior{
protected:
	QPair<int,int> range;
	int state;
	int inc;
public:
	Mat4Behavior* mainBehavior;
	AlternateBehavior(QPair<int,int> range, Mat4Behavior* mainBehavior=NULL);
	AlternateBehavior(int tstart, int tend, Mat4Behavior* mainBehavior=NULL);
	QMatrix4x4 move(const QMatrix4x4& initial);
};

class Collider{
public:
	virtual bool intersectRay(QMatrix4x4 transform, QVector3D source,
							  QVector3D direction) const = 0;
};

class ColliderRect : public Collider{
	QVector4D u,v;
	QVector4D A;
public:
	ColliderRect(QVector3D u, QVector3D v, QVector3D A);
	virtual bool intersectRay(QMatrix4x4 transform,
							  QVector3D source, QVector3D direction) const;
};

class ColliderCube : public Collider{
	ColliderRect rect1;
	ColliderRect rect2;
public:
	ColliderCube(QVector3D o, QVector3D u, QVector3D v, QVector3D w);
	virtual bool intersectRay(QMatrix4x4 transform,
							  QVector3D source, QVector3D direction) const;

};

class MapElement
{
	friend class Map;
protected:
	QPoint mapPos;
	QMatrix4x4 transformM;
	Collider* collider;
	QList<Object3D*> _objects;
public:
	QList<Behavior*> behaviors;
	MapElement(QList<Object3D*> models, Collider* collider);
	bool intersectRay(QVector3D source, QVector3D direction);
	virtual void highlight();
	virtual void unhighlight();
	virtual void update(Level&);
	virtual void actionOnCollision(Player & p) = 0;
	virtual void actionOnClick(Player & p);
	virtual bool isDead() = 0;
	virtual bool blockPath() const = 0;

	float x() const;
	float y() const;
	float z() const;
	QVector3D position() const;

	void transform(const QMatrix4x4 &transformM);
	void transformRight(const QMatrix4x4 &transformM);
	void resetPosition();

	QList<Object3D *> objects() const;
	void resetTransformation();
};



#endif // MAPELEMENT_H
