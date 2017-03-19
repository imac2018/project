#ifndef MAP_H
#define MAP_H

#include <QMap>
#include <QString>

#include "object3d.h"


class Player;
class Level;
class Mode;
class Game;
class MapElement;

enum Orientation{
	NORTH=0,
	EAST,
	SOUTH,
	WEST,
	OrientationCount
};

class Case{
	friend class Map;
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
	virtual bool isReachable() const = 0;
	virtual bool isReachable(const Player& p, Level& l) const = 0;
	virtual void action(Player& p, Level& l) = 0;

	void setElement(MapElement* e);
	void removeElement();

	Type type() const;
	QPoint pos() const;
};

class PreviousCase : public Case{
public:
	PreviousCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
	bool isReachable() const;
};

class EmptyCase : public Case{
public:
	EmptyCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
	bool isReachable() const;
};

class WallCase : public Case{
public:
	WallCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
	bool isReachable() const;
};

class NextCase : public Case{
public:
	const Orientation direction;
	NextCase(int x, int y, Orientation direction);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
	bool isReachable() const;
};

class DoorCase : public Case{
	friend class DoorElement;
	Orientation direction;
public:
	bool locked;
	DoorCase(Orientation direction, int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
	bool isReachable() const;
};

class WaterCase : public Case{
public:
	WaterCase(int x, int y);
	bool isReachable(const Player &p, Level& l) const;
	void action(Player &p, Level &l);
	bool isReachable() const;
};

class Map
{
	enum ModelType{
		WallForward=0,
		WallBackward,
		WallLeft,
		WallRight,
		Water,
		Ground,
		Stairs,
		Door,
		DoorBorder,
		Ceiling,
		ModelTypeCount
	};
	enum TextureType{
		T_Wall0=0,
		T_Wall1,
		T_Wall2,
		T_Wall3,
		T_Wall4,
		T_Comptoir,
		T_Ground1,
		T_Ground2,
		T_Ground3,
		T_Ground4,
		T_Ceiling1,
		T_Ceiling2,
		TextureTypeCount
	};
	Case*** cases;


	Orientation _initialOrientation;
	PreviousCase* entry;
	NextCase* exit;

	Level& parent;

	int width, height;
	QVector<QList<Object3D*> > models;
	QVector<QOpenGLTexture*> textures;
	QList<Object3D*> objects;
	QList<MapElement*> elements;
	MapElement* hoveredElement;

	float initialisationState;
	int icase;

	void loadCase(int x, int y, QColor c);
	float initializeMapModels(Game & game);
	float initializeMapObjects();
	void addWalls(int x, int y);
	void makeCeiling();
	void makeDeepWall();

public:
	const QSizeF caseSize;
	Map(Level& level);
	void loadMap(const QString& filepath);
	float initialize(Game & game);
	QVector3D caseCenter(int x, int y) const;
	const Case& getCase(int x, int y) const;
	const Case& getCase(QPoint p) const;

	void placeElement(MapElement* e, int x, int y);

	void moveForward(Player& p);
	void moveBack(Player& p);

	bool handleMouseRay(QVector3D source, QVector3D direction);
	bool handleMouseClick(Player &p);

	void draw(Renderer& renderer) const;
    bool validCase(QPoint p) const;

	void update();
	void clear();

	QPoint initialMapPosition() const;
	QVector3D initialRealPosition() const;
	Orientation initialOrientation() const;

	static QPoint getMoveFromOrientation(Orientation o);
};

#endif // MAP_H
