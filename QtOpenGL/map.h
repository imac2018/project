#ifndef MAP_H
#define MAP_H

#include <QMap>
#include <QString>

#include "object3d.h"


class Player;
class Level;
class Mode;
class Game;

enum Orientation{
	NORTH=0,
	EAST,
	SOUTH,
	WEST,
	OrientationCount
};

class MapElement{

};

class Case{
public:
	enum Type{
		Empty,
		Wall,
		Door,
		Water,
		Next,
		Previous
	};
protected:
	Type _type;
	QPoint _pos;
public:
	Case(Type type, int x, int y);
	virtual bool isReachable(const Player& p, Level& l) const = 0;
	virtual void action(Player& p, Level& l) = 0;

	Type type() const;
	QPoint pos() const;
};

class PreviousCase : public Case{
public:
	PreviousCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
};

class EmptyCase : public Case{
public:
	EmptyCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
};

class WallCase : public Case{
public:
	WallCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
};

class NextCase : public Case{
public:
	NextCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
};

class DoorCase : public Case{
	Orientation direction;
public:
	bool locked;
	DoorCase(Orientation direction, int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
};

class WaterCase : public Case{
public:
	WaterCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
};

class Map
{
	enum ModelType{
		WallFaceDefault=0,
		WallFace2=0,
		WallFace3=0,
		WallSideDefault,
		WallSide2,
		WallSide3,
		GroundDefault,
		Ground2,
		Ground3,
		Ground4,
		CeilingDefault,
		Ceiling2,
		ModelTypeCount
	};
	Case*** cases;


	Orientation _initialOrientation;
	PreviousCase* entry;
	NextCase* exit;

	Level& parent;

	int width, height;
	QVector<Object3D*> models;
	QList<Object3D*> objects;

	float initialisationState;
	int icase;

	void loadCase(int x, int y, QColor c);
	void loadMap(const QString& filepath);
	float initializeMapModels(Game & game);
	float initializeMapObjects(Game & game);
	void addWalls(Game& game, int x, int y);
	void makeCeiling(Game& game);

public:
	const QSizeF caseSize;
	Map(const QString& filepath, Level& level);
	float initialize(Game & game);
	QVector3D caseCenter(int x, int y) const;
	Case& getCase(int x, int y);

	void moveForward(Player& p);
	void moveBack(Player& p);

	void draw(Renderer& renderer) const;

	QPoint initialMapPosition() const;
	QVector3D initialRealPosition() const;
	Orientation initialOrientation() const;

	static QPoint getMoveFromOrientation(Orientation o);
};

#endif // MAP_H
