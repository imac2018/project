#ifndef PLAYER_H
#define PLAYER_H

#include <QInputEvent>
#include <QMap>
#include <functional>
#include "galata.h"
#include "ship.h"

class Input{
	static long makeHashcode(const QInputEvent* e);
	const QInputEvent event;
	const long _hashcode;
public:
	Input(const QInputEvent* e);
	Input(const QInputEvent& e);
	inline bool testInput(const Input& i2) const { *this == i2; }
	virtual bool operator==(const Input& i2) const;
	virtual bool operator!=(const Input& i2) const;
	virtual bool operator<(const Input& i2) const;
	long hashcode() const;
};

#define ShipAction std::function<void(Ship&)>

class Player
{
	QMap<Input, ShipAction> controls;
	Ship* ship;
	unsigned int _points;
	QString _name;
public:
	Player(QString _name);
	void setShip(Ship* ship);
	void addShipToGalata(Galata& level);
	void addControl(Input control, ShipAction shipAction);
	virtual bool inputHandle(QInputEvent *event);
	void addPoints(unsigned int _points);
	unsigned int points() const;
	QString name() const;

};

#endif // PLAYER_H
