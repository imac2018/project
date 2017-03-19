#ifndef ENEMY_H
#define ENEMY_H

#include "mapelement.h"

class Enemy : public MapElement
{
	int life;
	int t;
	int cycle;

	Orientation _direction;
	int targetDirection;
	float initialY;

	float rotationValue;
	float rotationSpeed;
	float rotationTarget;

	float translateSpeed;
	QVector3D translateTarget;
	Map* map;

	int findPlayer(Level &l, int x, int y, int distance,
				   QPoint prevPos);
public:
	Enemy(QList<Object3D *> models, Collider *collider, Map* map,
		  float initialY, int life=5, Orientation direction=NORTH);
	void setCycle(int cycle);

	void update(Level &l);
	void actionOnCollision(Player &p);
	void actionOnClick(Player &p);
	bool isDead();
	bool blockPath() const;

	void moveForward();
	void moveBack();
	void setPosition(QPoint mapPos, QVector3D realPos,
					 Orientation dir = NORTH);

	void rotateLeft();
	void rotateRight();

	Orientation direction() const;

};

#endif // ENEMY_H
