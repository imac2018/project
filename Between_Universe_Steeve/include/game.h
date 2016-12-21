#ifndef GAME_H
#define GAME_H

#include <qvector.h>
#include <QSettings>
#include <QSet>

#include "character.h"
#include "ship.h"
#include "player.h"
#include "configdialog.h"

/**
 * @brief The main class, it manage the whole program by switching between the different mode
 * Unfortunately, for the moment it manage only the Galata mode
 * It has the common elements of the program as the characters, the ships, the weapons...
 */
class Game : public QObject
{
	Q_OBJECT

	static QVector<Character*> characters;
	static QVector<Player> defaultPlayers;
	static QMap<QString,Ship*> shipModels;
	static QMap<QString,Weapon*> weaponModels;
	static QMap<QString, ShipAction> availableAction;
	static QVector<QVector<QPair<Input, ShipAction> > > defaultInputs;

	static Qt::Key breakKey;
	static Qt::Key configKey;

	QSettings settings;
	QVector<Player*> players;

	QString currentLevelFile;

	Mode* current;
	Mode* next;

	ConfigDialog* configDialog;

	float transitionState;
	float transitionInc;

public:
	Game();
	/**
	 * @brief init all the common element
	 */
	static void staticInit();
	/**
	 * @brief reload a galata level from the current setting
	 */
	void reload();
	/**
	 * @brief add a new player from the default model
	 * @param true if the player will be present in the next reload
	 */
	void addDefaultPlayer(bool enabled);
	/**
	 * @brief Add to the player the differents control from the default input
	 * @return true if the default inputs have correctly been set
	 */
	bool setDefaultInputToPlayer(Player* p);
	/**
	 * @return true if the Input is already reserved by the program or by a player
	 */
	bool inputIsUsed(const Input& i) const;
	void setCurrentMode(Mode* current);
	bool updateGame();

	void showConfigDialog();

	static QStringList shipNames();
	static QStringList weaponNames();

	static Ship* shipModelFromName(QString name);
	static Weapon* weaponModelFromName(QString name);

	const Renderer* currentRenderer();
protected:
	bool eventFilter(QObject *, QEvent *event);

public slots:
	/**
	 * @brief slot triggered after the configDialog close
	 */
	void setConfig();

signals:
	/**
	 * @brief signal emitted when the mode as changed (when we should change the current renderer)
	 */
	void modeHasChanged();
};

#endif // GAME_H
