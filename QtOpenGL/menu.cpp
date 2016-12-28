#include "menu.h"
#include "level.h"
#include "renderer.h"

#include <QPixmap>
#include <QMouseEvent>

Menu::Menu()
{
}

void Menu::initialize(Game &game, float &currentState)
{
	Renderer& renderer = game.renderer();

	if(currentState<0.1f){
		renderer.resetGlobal();
		renderer.projectionType = Renderer::Ortho;
		game.updateProjection();
		currentState = 0.25f;
	}
	else if(currentState<0.4f){
		gui.append(new ChangeModeButton(game, new Level(),
										renderer,"Yolo",QRectF(-0.25f,-0.75f,0.5f,0.1f)));

		objects.append(gui.objects());
		currentState = 0.5f;
	}
	else if(currentState<0.6){
		currentState+=0.004f;
	}
	else if(currentState<0.75f){
		QList<Object3D*>::iterator j;
		for(j=objects.begin();j!=objects.end();j++){
			renderer.addObject(**j);
		}
		currentState = 0.75f;
	}
	else if(currentState<0.95f){
		renderer.initializeVBO();
		currentState = 0.95f;
	}
	else{
		renderer.initializeVAO();
		renderer.resetGlobal();
		currentState = 1;
	}
}

bool Menu::inputHandle(Game &, QInputEvent * e)
{
	QMouseEvent* mouseE = dynamic_cast<QMouseEvent*>(e);
	if(mouseE){
		if(mouseE->type() == QEvent::MouseMove){
			return gui.handleMouseMove(mouseE);
		}
		else if(mouseE->type() == QEvent::MouseButtonPress){
			return gui.handleMousePress(mouseE);
		}
		else if(mouseE->type() == QEvent::MouseButtonRelease){
			return gui.handleMouseRelease(mouseE);
		}
	}
	return false;
}

void Menu::update(Game &)
{

}

void Menu::render(Game &game)
{
	Renderer& renderer = game.renderer();
	renderer.projectionType = Renderer::Ortho;
	game.updateProjection();
	renderer.initRender();
	renderer.draw(objects);
	renderer.endRender();
}

void Menu::clear(Game &game)
{
	Renderer& renderer = game.renderer();
	renderer.clearVAO();
	renderer.clearVBO();
	renderer.clearTextures();
}



