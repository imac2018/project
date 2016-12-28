#ifndef MENU_H
#define MENU_H

#include "mode.h"
#include "object3d.h"
#include "game.h"
#include "gui.h"

class Menu : public Mode
{
private:
	QList<Object3D*> objects;
	Gui gui;
	// Mode interface
public:
	Menu();
	void initialize(Game &, float &currentState);
	bool inputHandle(Game &, QInputEvent *);
	void update(Game &);
	void render(Game &);
	void clear(Game &);
};

#endif // MENU_H
