#ifndef PLAYER_H
#define PLAYER_H

#include <QInputEvent>
#include <QMap>
#include <functional>
#include "galata.h"
#include "ship.h"

class QSettings;

/**
 * @brief This object is used as a key (like in a QMap), it define an hashcode from a QInputEvent
 * It allow to seek an action triggered by a specific input
 */
class Input{
	static long makeHashcode(const QInputEvent* e);
	QInputEvent event;
	long _hashcode;
public:
	Input();
	Input(const QInputEvent* e);
	Input(const QInputEvent& e);
	inline bool testInput(const Input& i2) const { *this == i2; }
	virtual bool operator==(const Input& i2) const;
	virtual bool operator!=(const Input& i2) const;
	virtual bool operator<(const Input& i2) const;
	Input& operator= (Input i2);

	long hashcode() const;
	void save(QSettings& settings, const QString& prefix);
	static Input load(const QSettings& settings, const QString& prefix);
};

/**
 * @brief action handled by a ship
 */
typedef std::function<void(Ship&)> ShipAction;

/**
 * @brief A Player is the user of the game, he has a name and a color.
 * For a specific level, he has a ship and handle the event to control it
 */
class Player
{
	QMap<Input, ShipAction> controls;
	Ship* currentShip;
	unsigned int _points;
	QString _name;
	QColor _color;
	int _life;
	Galata* level;
	bool inGame;

public:
	const Ship* shipModel;
	const Weapon* mainWeapon;
	const Weapon* secondWeapon;

	Player();
	Player(QString _name, QColor _color, int _life, bool inGame=true);
	void setShip(const Ship& shipModel);
	void setMainWeapon(const Weapon& w);
	void setSecondWeapon(const Weapon& w);
	void setLevel(Galata* level);
	void addShipToGalata();
	void handleShipDeath();
	void addControl(const Input& control, ShipAction shipAction);
	void removeAction(ShipAction action);
	virtual bool inputHandle(QInputEvent *event);
	void addPoints(unsigned int _points);

	void setName(const QString& name);
	void setColor(const QColor& color);
	void setLife(int life);
	void putInGame(bool inGame);

	bool canHandleInput(const Input& i) const;
	unsigned int points() const;
	QString name() const;
	QColor color() const;
	int life() const;
	bool isInGame() const;

};

#endif // PLAYER_H
