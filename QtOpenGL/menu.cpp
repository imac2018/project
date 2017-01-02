#include "menu.h"
#include "level.h"
#include "renderer.h"
#include "audio.h"

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
		gui.appendElement(new Image(renderer,QRectF(-1,-1,2,2),":/assets/images/magasin.png"));
		currentState = 0.4f;
	}
	else if(currentState<0.5f){
		gui.appendBtn(new ChangeModeButton(game, new Level(":/level/level1.xml",*this),
										   renderer, ":/assets/images/start.png",
											 QRectF(0.23f,0.15f,0.3f,0.1f)));
		currentState = 0.5f;
	}
	else if(currentState<0.6){
		currentState+=0.004f;
	}
	else if(currentState<0.7f){
		Dialog* exitDialog = new Dialog();
		exitDialog->initialize(renderer,
							   new ExitButton(renderer,"Exit",
											   exitDialog->confirmBtnBounds(), true, Dialog::defaultBtnFontSize),
							   "Do you really\n want\n to exit?");
		Button* b = new ToggleDialogButton(exitDialog,true, renderer, ":/assets/images/exit.png",
											 QRectF(0.23f,0.5f,0.3f,0.1f));
		exitDialog->hide();
		gui.appendBtn(b);
		exitDialog->appendToGui(gui);
		currentState=0.7f;
	}
	else if(currentState<0.75f){
		gui.addObjectsToRenderer(renderer);
		currentState = 0.75f;
	}
	else if(currentState<0.95f){
		renderer.initializeVBO();
		currentState = 0.95f;
	}
	else if(currentState<0.99f){
		renderer.initializeVAO();
		renderer.resetGlobal();
		currentState = 0.99f;
	}
	else{
		AudioPlayer* player = AudioManager::player("generique.mp3");
		if(player)
			player->play();
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

void Menu::render(Game &game) const
{
	Renderer& renderer = game.renderer();
	renderer.projectionType = Renderer::Ortho;
	game.updateProjection();
	renderer.initRender();
	gui.render(game);
	renderer.endRender();
}

void Menu::clear(Game &game)
{
	Renderer& renderer = game.renderer();
	AudioPlayer* player = AudioManager::player("generique.mp3");
	if(player)
		player->stop();
	renderer.clearVAO();
	renderer.clearVBO();
	renderer.clearTextures();
	gui.clear();
}



