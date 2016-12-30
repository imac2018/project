#ifndef GAME_H
#define GAME_H

#include "renderer.h"

class Mode;
class QWidget;

class Game : public QObject
{
	enum State{
		Normal,
		TransitionOut,
		TransitionIn,
		Loading
	};

	QWidget* mainWindow;

	Mode* current;
	Mode* next;

	Mode* _firstMode;

	State state;
	float loadingState;
	Renderer loadingRenderer;
	Renderer _renderer;

	Object3D* loadingDonut;
	QMatrix4x4 rotationMatrix;

	int windowWidth, windowHeight;

	QRectF currentBound;

	void transitionOutUpdate();
	void transitionInUpdate();
	void loadingUpdate();

	void renderLoading();

	QPointF windowToGL(QPointF p);

public:
	Game(QWidget* view, Mode* _firstMode);
	void initializeRender();
	void changeMode(Mode* m);

	void updateGame();
	void render();

	void resizeHandler(int windowWidth, int windowHeight);
	void updateProjection();

	Renderer& renderer();

	Mode* firstMode();
protected:
	bool eventFilter(QObject *, QEvent *event);
};

#endif // GAME_H
