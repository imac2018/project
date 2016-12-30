#include "level.h"
#include "game.h"
#include "menu.h"
#include "object3d.h"
#include "tools.h"
#include <QMouseEvent>

void Level::loadModels()
{
	QList<Object3D*> objects = Object3D::importFromFile(":/models/graou.obj");
	if(objects.length()){
		models[Beer] = objects.first();
	}
}

Level::Level(Mode &previous, Mode *next)
	: previous(previous), next(next),
	  player(*this, &map), map(":/level/map1.ppm", *this),
	  t(0), tInc(0.005), models(ModelsCount)
{
	loadModels();
}

void Level::initialize(Game & game, float &currentState)
{
	Renderer& renderer = game.renderer();

	if(currentState<0.1f){
		renderer.resetGlobal();
		renderer.projectionType = Renderer::Ortho;
		game.updateProjection();
		dirLight.color = QColor::fromRgbF(0.75,0.47,0.7);
		dirLight.direction = QVector3D(-0.3,-0.8,-0.5).normalized();
		torchLight.power = 0.75f;
		torchLight.color = QColor::fromRgbF(1,0.75,0.8);
		transformLight.setToIdentity();
		transformLight.rotate(1,1,0,0);
		currentState = 0.1f;
	}
	else if(currentState<0.2f){
		if(next != NULL){
			nextDialog.initialize(renderer,
								  new ChangeModeButton(game, next,
														 renderer,"Yes!",nextDialog.confirmBtnBounds()),
								  "Do you want to go to next step ?");
		}
		else{
			nextDialog.initialize(renderer,
								  new ChangeModeButton(game, game.firstMode(),
														 renderer,"Yes!",nextDialog.confirmBtnBounds()),
								  "Congratulation! You finish the Game!"
								  "\nDo you want to return to the menu?");
		}
		currentState = 0.2f;
	}
	else if(currentState<0.3){
		previousDialog.initialize(renderer,
							  new ChangeModeButton(game, &previous,
												   renderer,"Yes...",nextDialog.confirmBtnBounds()),
							  "Do you want to go back ?");
		currentState = 0.3f;
	}
	else if(currentState<0.4f){

		exitDialog.initialize(renderer,
							  new ChangeModeButton(game,game.firstMode(),renderer,
												   "Yes...",nextDialog.confirmBtnBounds()),
							  "Do you want to exit ?");
		currentState = 0.4f;
	}
	else if(currentState<0.5f){

		nextDialog.appendToGui(playGui);
		previousDialog.appendToGui(playGui);
		exitDialog.appendToGui(playGui);
		hideDialog();
		currentState = 0.5f;
	}
	else if(currentState<0.55){
		playGui.appendBtn(new ToggleDialogButton(&exitDialog,true,renderer,
												 "Exit",QRectF(-0.9,-0.9,0.3,0.1)));
		currentState = 0.55f;
	}
	else if(currentState<0.6){
		currentState+=0.01f;
	}
	else if(currentState<0.75f){
		currentState = 0.6f + map.initialize(game)*0.15f;
	}
	else if(currentState<0.95f){
		playGui.addObjectsToRenderer(renderer);
		foreach(Object3D* o, models){
			renderer.addObject(*o);
		}
		currentState = 0.96f;
	}
	else{
		player.setPosition(map.initialMapPosition(),
						   map.initialRealPosition(),
						   map.initialOrientation());
		renderer.initializeVBO();
		renderer.initializeVAO();
		renderer.resetGlobal();
		currentState = 1;
	}
}

bool Level::inputHandle(Game & game, QInputEvent * e)
{
	QMouseEvent* mouseE = dynamic_cast<QMouseEvent*>(e);
	if(mouseE){
		if(mouseE->type() == QEvent::MouseMove){
			return playGui.handleMouseMove(mouseE);
		}
		else if(mouseE->type() == QEvent::MouseButtonPress){
			return playGui.handleMousePress(mouseE);
		}
		else if(mouseE->type() == QEvent::MouseButtonRelease){
			return playGui.handleMouseRelease(mouseE);
		}
	}
	return player.inputHandle(game,e);
}

void Level::update(Game & game)
{
	t+=tInc;
	if(t>1.f){
		t=1.f;
		tInc*=-1;
		transformLight.setToIdentity();
		transformLight.rotate(1,1,0,0);
	}
	else if(t<0.f){
		t=0.f;
		tInc*=-1;
		transformLight.setToIdentity();
		transformLight.rotate(-1,1,0,0);
	}

	dirLight.direction = (transformLight * QVector4D(dirLight.direction)).toVector3D();
	player.update(game);
	playGui.update();
}

void Level::render(Game & game) const
{
	Renderer& renderer = game.renderer();
	renderer.initRender();

	glEnable(GL_DEPTH_TEST);
	renderer.projectionType = Renderer::Perspective;
	game.updateProjection();
	renderer.bindShader(Renderer::Normal);
	renderer.bindCamera(player.camera());
	renderer.bindDirectionalLight(dirLight);
	renderer.bindTorchLight(torchLight);
	renderer.draw(objects);
	map.draw(renderer);
	Object3D o(*models[Beer]);
	o.transformM.translate(map.caseCenter(5,17));
	o.transformM.scale(2);
	renderer.draw(&o);

	playGui.render(game);

	renderer.endRender();
}

void Level::clear(Game & game)
{
	Renderer& renderer = game.renderer();
	renderer.clearVAO();
	renderer.clearVBO();
	renderer.clearTextures();
}

void Level::askPreviousMode()
{
	previousDialog.show();
}

void Level::askNextMode()
{
	nextDialog.show();
}

void Level::hideDialog()
{
	nextDialog.hide();
	previousDialog.hide();
	exitDialog.hide();
}
