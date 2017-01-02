#ifndef PLAYER_H
#define PLAYER_H

#include <QInputEvent>

#include "map.h"
#include "camera.h"

class Game;
class Renderer;

class Player
{
	Camera view;
	Orientation _direction;

	float rotationSpeed;
	float rotationTarget;

	float translateSpeed;
	QVector3D translateTarget;
	Case* caseTarget;

	int _life;

	float upSpeed;
	float initialY;

	int _duffCount;

	QPoint mapPos;

	Map* map;
	Level& parent;

public:
	Player(Level& parent, Map* map = NULL);
	void setMap(Map* map);
	bool inputHandle(Game &, QInputEvent *e);
	void update(Game &);

	void moveForward(Case* target);
	void moveBack(Case *target);
	void setPosition(QPoint mapPos, QVector3D realPos,
					 Orientation dir = NORTH);

	void rotateLeft();
	void rotateRight();

	void incrementDuff();

	QPoint mapPosition() const;
	QVector3D realPosition() const;
	int duffCount() const;
	int life() const;
	const Camera& camera() const;

	void receiveDamage();

	void clear();

	Orientation direction() const;
};

#endif // PLAYER_H
