#ifndef GALATA_H
#define GALATA_H

#include "mode.h"
#include "renderer.h"
#include "space.h"
#include "item.h"
#include "enemy.h"

class Player;
class GalataRenderer;
class Ship;
class QFile;
class QDomElement;
class Item;

/**
 * @brief This mode define a level of the Game
 */
class Galata : public Mode
{
	friend class GalataRenderer;
private:
	/**
	 * @brief This Renderer is used to render a level
	 */
	class GalataRenderer : public Renderer{
		Galata* parent;
	public:
		GalataRenderer(Galata* parent);
	protected:
		virtual void paint(QPainter& context) const;
	private:
		void drawCenteredMessage(QPainter &context, QString message,
								 int pointSize, bool bold, QColor color) const;
		void drawCenteredMessage(QPainter &context, QString message,
								 int pointSize, bool bold, QColor color,
								 int tx, int ty) const;
	};

	QList<Player*> players;
	Space _space;
	QPointF _initialShipPosition;
	int nbPlayer;
	bool _break;
	bool end;
	float tEnd;
	float tInc;
	float opacity;
public:
	Galata(Player* p1);
	Galata(QFile& file, Player *p1);
	void addPlayer(Player* p);
	virtual bool inputHandle(QInputEvent *event);
	QPointF initialShipPosition();
	void addShip(Ship* ship);
	void addEnemy(Enemy* e);
	void addItem(Item* i);
	void handlePlayerDeath(Player* p);

	void load(QFile& file);
	void loadElement(const QDomElement& e);
	PositionRange loadRange(const QDomElement& e);
	Enemy* loadEnemy(const QDomElement& e);
	Item* loadItem(const QDomElement& e);
	Behavior* loadBehavior(const QDomElement& e);
	void loadEnemySupplement(Enemy* enemy,const QDomElement& e);

	const Space& space();
	static QImage spaceBorder;

	~Galata();

public slots:
	virtual bool update();
	bool updateStartTransition(float t);
	bool updateEndTransition(float t);
};

#endif // GALATA_H
