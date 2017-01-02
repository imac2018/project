#ifndef LEVEL_H
#define LEVEL_H

#include "mode.h"
#include "gui.h"
#include "player.h"
#include "map.h"
#include "renderer.h"
#include <QtXml/QDomElement>

class Camera;

class Level : public Mode
{
public:
	enum ModelType{
		Beer = 0,
		Beer2D,
		Donut2D,
		Ned2D,
		ModelsCount
	};
private:
	QString levelFilePath;

	Player _player;
	Map _map;

	Gui playGui;

	QVector<QList<Object3D*> > models;
	QList<Object3D*> objects;

	Dialog nextDialog;
	Dialog previousDialog;
	Dialog exitDialog;
	Dialog gameOverDialog;

	int _condition;
	Label* conditionLbl;
	Label* wallLbl;
	Label* doorInstructionLbl;

	Mode& previous;
	Mode* next;

	DirectionalLight dirLight;
	TorchLight torchLight;	

	QMatrix4x4 transformLight;
	float t;
	float tInc;

	void loadModels();
	void loadLevel(QString levelFilepath);
	void loadElement(const QDomElement& e);
public:
	Level(QString levelFilePath, Mode &previous);

	void initialize(Game &, float &currentState);
	bool inputHandle(Game &, QInputEvent *e);
	void update(Game &);
	void render(Game &) const;
	void clear(Game &);

	void askPreviousMode();
	void askNextMode();

	virtual bool testCondition(const Player& p);
	void displayCondition();
	void displayWallMsg();
	void displayDoorMsg();
	void displayGameOver();

	const Map& map();
	Player& player();

	void hideDialog();
	bool dialogMode() const;

	int condition() const;

};

#endif // LEVEL_H
