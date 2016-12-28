#ifndef LEVEL_H
#define LEVEL_H

#include "mode.h"
#include "gui.h"

class Camera;

class Level : public Mode
{
	enum Orientation{
		NORTH=0,
		EAST,
		SOUTH,
		WEST,
		OrientationCount
	};

	enum ModelObject{
		WallFace,
		WallSide,
		Ground
	};

	Gui playGui;
	QVector<Object3D*> models;
	QList<Object3D*> objects;

	Orientation direction;

	float rotationSpeed;
	float rotationTarget;

	float translateSpeed;
	QVector3D translateTarget;

	float upSpeed;
	float initialY;

	bool testTargetPos(const Camera& c) const;
public:
	Level();

	// Mode interface
public:
	void initialize(Game &, float &currentState);
	bool inputHandle(Game &, QInputEvent *e);
	void update(Game &);
	void render(Game &);
	void clear(Game &);
};

#endif // LEVEL_H
