#ifndef LEVEL_H
#define LEVEL_H

#include "mode.h"
#include "gui.h"
#include "player.h"
#include "map.h"
#include "renderer.h"

class Camera;

class Level : public Mode
{
public:
	enum ModelType{
		Beer = 0,
		ModelsCount
	};
private:
	Player player;
	Map map;

	Gui playGui;

	QVector<Object3D*> models;
	QList<Object3D*> objects;

	Dialog nextDialog;
	Dialog previousDialog;
	Dialog exitDialog;

	Mode& previous;
	Mode* next;

	DirectionalLight dirLight;
	TorchLight torchLight;

	QMatrix4x4 transformLight;
	float t;
	float tInc;

	void loadModels();

public:
	Level(Mode &previous, Mode* next);

	void initialize(Game &, float &currentState);
	bool inputHandle(Game &, QInputEvent *e);
	void update(Game &);
	void render(Game &) const;
	void clear(Game &);

	void askPreviousMode();
	void askNextMode();

	void hideDialog();
};

#endif // LEVEL_H
