#ifndef MAP_H
#define MAP_H

#include <QMap>
#include <QString>

#include "object3d.h"

class Position{
	int x,y;
	int hascode;
public:
	Position(int x, int y);
	bool operator<(const Position& p) const;
	bool operator!=(const Position& p) const;
};

class Map
{
	QMap<Position, Object3D*> wall;
public:
	Map();
	bool loadMap(const QString& filepath);
};

#endif // MAP_H
