#include "level.h"
#include "game.h"
#include "menu.h"
#include "object3d.h"
#include "tools.h"
#include "item.h"
#include "enemy.h"
#include "audio.h"
#include "glexception.h"
#include <QMouseEvent>
#include <QtXml/QDomDocument>
#include <QtXml/QXmlParseException>

void Level::loadModels()
{
	QList<Object3D*> objects = Object3D::importFromFile(":/models/duff.3ds");
	if(objects.length()){
		foreach(Object3D* o, objects){
			o->rotate(70,50,25);
			o->scale(2.2f);
		}
		models[Beer] = objects;
	}
}

void Level::loadElement(const QDomElement &e)
{
	if(e.tagName() == "enemy"){
		int type = e.attribute("type",QString::number(Ned2D)).toInt();
		Enemy* enemy = new Enemy(models[type],
				new ColliderRect(QVector3D(0,_map.caseSize.height(),0),
								 QVector3D(_map.caseSize.height()*0.8,0,0),
								 QVector3D(-_map.caseSize.height()*0.4,
										   -_map.caseSize.height()*0.5,0)),
								 &_map,
								 _map.caseSize.height()*0.35,
								 e.attribute("life","5").toInt(),
								 (Orientation)e.attribute("orientation","0").toInt());
		if(e.hasAttribute("cycle"))
			enemy->setCycle(e.attribute("cycle").toInt());
		_map.placeElement(enemy, e.attribute("x").toInt(), e.attribute("y").toInt());
	}
	else if(e.tagName() == "duff"){
		DuffItem* item = new DuffItem(models[Beer],
									 new ColliderCube(QVector3D(-1,-1,-1),
													  QVector3D(2,0,0),
													  QVector3D(0,2,0),
													  QVector3D(0,0,2)));
		_map.placeElement(item, e.attribute("x").toInt(), e.attribute("y").toInt());
	}
	else if(e.tagName() == "torch"){
		torchLight.power = e.attribute("power","1").toFloat();
		torchLight.color = QColor(e.attribute("r","255").toShort(),
												  e.attribute("g","255").toShort(),
												  e.attribute("b","255").toShort());

	}
	else if(e.tagName() == "dirlight"){
		dirLight.color = QColor(e.attribute("r","255").toShort(),
								e.attribute("g","255").toShort(),
								e.attribute("b","255").toShort());
		dirLight.direction = QVector3D(e.attribute("x","0").toFloat(),
									   e.attribute("y","-1").toFloat(),
									   e.attribute("z","0").toFloat());
		transformLight.setToIdentity();
		transformLight.rotate(e.attribute("rotationx","0").toFloat(),1,0,0);
		transformLight.rotate(e.attribute("rotationy","0").toFloat(),0,1,0);
		transformLight.rotate(e.attribute("rotationz","0").toFloat(),0,0,1);
	}
}

void Level::loadLevel(QString levelFilepath){
	QFile file(levelFilepath);
	if(!file.open(QIODevice::ReadOnly |
				  QIODevice::Text)){
		throw GLException("Level Import Error", "The file " + file.fileName() +
							 " can't be read\n" + file.errorString());
	}
	QDomDocument d;
	QDomElement root;
	try{
		if(!file.seek(0))
			throw GLException("Level Import Error","error while reading the file " +
										 file.fileName() + " : " + file.errorString());
		QByteArray content = file.readAll();
		d.setContent(content);

		root = d.documentElement();
		_map.loadMap(root.attribute("mappath"));
		if(root.hasAttribute("nextlevelpath")){
			next = new Level(root.attribute("nextlevelpath"), *this);
		}
		if(root.hasAttribute("condition")){
			_condition = root.attribute("nextlevelpath").toInt();
		}

		dirLight.color = QColor::fromRgbF(0.5f,0.47f,0.53f);
		dirLight.direction = QVector3D(-0.3f,-0.8f,-0.5f).normalized();
		torchLight.power = 1.2f;
		torchLight.color = QColor::fromRgbF(1,0.75f,0.8f);
		transformLight.setToIdentity();

		QDomNodeList nodes = root.childNodes();
		for(int i=0; i<nodes.length();i++)
		{
			loadElement(nodes.at(i).toElement());
		}
	}
	catch (QXmlParseException& e){
		file.close();
		throw GLException("Level Import Error", "An error has been met with the file "
							 + file.fileName() + " at:" + e.lineNumber() +
							 ":" + e.columnNumber() +"\n"+ e.message());
	}
	file.close();
}

Level::Level(QString levelFilePath, Mode &previous)
	: levelFilePath(levelFilePath), previous(previous), next(NULL),
	  _player(*this, &_map), _map(*this),
	  t(0), tInc(0.005f), models(ModelsCount),
	  conditionLbl(NULL), _condition(3)
{

}

void Level::initialize(Game & game, float &currentState)
{
	Renderer& renderer = game.renderer();
	if(currentState<0.1f){
		renderer.resetGlobal();
		renderer.projectionType = Renderer::Ortho;
		game.updateProjection();
		currentState = 0.1f;
	}
	else if(currentState<0.12f){
		models[Beer2D].append(new Image(renderer,QRectF(-0.025f,-0.035f,0.05f,0.07f),
										":/assets/images/duff.png"));
		models[Donut2D].append(new Image(renderer,QRectF(-0.025f,-0.025f,0.05f,0.05f),
										 ":/assets/images/loading.png"));
        models[Ned2D].append(new Object3D(renderer.makeTexturedFace(0,180,0,
																	0,0,0,
																	_map.caseSize.width()*0.2f,
																	_map.caseSize.height()*0.4f)));
		models[Ned2D].first()->texture = renderer.addTexture(":/assets/images/ned.png");
		currentState = 0.12f;
	}
	else if(currentState<0.15f){
		QList<Object3D*> objects = Object3D::importFromFile(":/models/duff.3ds");
		if(objects.length()){
			foreach(Object3D* o, objects){
				o->rotate(70,50,25);
				o->scale(2.2f);
			}
			models[Beer] = objects;
		}
		models[Beer].at(4)->texture = renderer.addTexture(":/assets/images/duff3D.jpg");
		currentState = 0.15f;
	}
	else if(currentState<0.17f){
		foreach(QList<Object3D*> objects, models){
			foreach(Object3D* o, objects){
				if(o->vertex.length())
					renderer.addObject(*o);
			}
		}
		currentState = 0.17f;
	}
	else if(currentState < 0.185f){
		loadLevel(levelFilePath);
		currentState = 0.185f;
	}
	else if(currentState<0.2f){
		if(next != NULL){
			nextDialog.initialize(renderer,
								  new ChangeModeButton(game, next,
														 renderer,"Yes!",nextDialog.confirmBtnBounds(), true, Dialog::defaultBtnFontSize),
								  "Do you want to \ngo to next step ?");
		}
		else{
			nextDialog.initialize(renderer,
								  new ChangeModeButton(game, game.firstMode(),
														 renderer,"Yes!",nextDialog.confirmBtnBounds(),
													   true, Dialog::defaultBtnFontSize),
								  "Congratulation! \nYou finish the Game!"
								  "\nDo you want to \nreturn to the menu?");
		}
		currentState = 0.2f;
	}
	else if(currentState<0.3){
		previousDialog.initialize(renderer,
							  new ChangeModeButton(game, &previous,
												   renderer,"Yes...",nextDialog.confirmBtnBounds(), true, Dialog::defaultBtnFontSize),
							  "Do you want\n to go back ?");
		currentState = 0.3f;
	}
	else if(currentState<0.35f){
		doorInstructionLbl = new Label(renderer,"Click on the door to Open",
								 QRectF(-0.5,-0.1,1,0.2),35, Qt::white, Qt::black);
		renderer.addObject(*doorInstructionLbl);
		currentState = 0.35;
	}
	else if(currentState<0.4f){

		exitDialog.initialize(renderer,
							  new ChangeModeButton(game,game.firstMode(),renderer,
												   "Yes...",nextDialog.confirmBtnBounds(), true, Dialog::defaultBtnFontSize),
							  "Do you want to exit ?");
		currentState = 0.4f;
	}
	else if(currentState<0.42f){
		gameOverDialog.initialize(renderer,
								  new ChangeModeButton(game,this,
													   renderer,"Fine...",gameOverDialog.confirmBtnBounds(), true, Dialog::defaultBtnFontSize),
								 "Game Over\nFlanders got you");
		currentState = 0.42f;
	}
	else if(currentState<0.45f){

		conditionLbl = new Label(renderer,"You need "+QString::number(_condition)+" Duff Before",
								 QRectF(-0.5,-0.1,1,0.2), 35, Qt::white, Qt::black);
		renderer.addObject(*conditionLbl);
		currentState = 0.45f;
	}
	else if(currentState<0.475f){
		wallLbl = new Label(renderer,"In fact there is a wall...",
								 QRectF(-0.5,-0.1,1,0.2), 35, Qt::white, Qt::black);
		renderer.addObject(*wallLbl);
		currentState = 0.475f;
	}
	else if(currentState<0.48f){
		playGui.appendBtn(new ToggleDialogButton(&exitDialog,true,renderer,
												 "Exit",QRectF(-0.9,-0.9,0.3,0.1),true,100));
		currentState=0.48f;
	}
	else if(currentState<0.5f){
		nextDialog.appendToGui(playGui);
		previousDialog.appendToGui(playGui);
		exitDialog.appendToGui(playGui);
		gameOverDialog.appendToGui(playGui);
		hideDialog();
		currentState = 0.5f;
	}
	else if(currentState<0.6){
		currentState+=0.01f;
	}
	else if(currentState<0.75f){
		currentState = 0.6f + _map.initialize(game)*0.15f;
	}
	else if(currentState<0.95f){
		playGui.addObjectsToRenderer(renderer);
		currentState = 0.96f;
	}
	else if(currentState<0.97f){
		currentState += 0.002f;
	}
	else if(currentState<0.985f){
		_player.setPosition(_map.initialMapPosition(),
						   _map.initialRealPosition(),
						   _map.initialOrientation());
		renderer.initializeVBO();
		renderer.initializeVAO();
		renderer.resetGlobal();
		currentState = 0.99;
	}
	else{
		AudioPlayer* player = AudioManager::player("apu.mp3");
		if(player){
			player->setVolume(30);
			player->play();
		}
		currentState = 1;
	}
}

bool Level::inputHandle(Game & game, QInputEvent * e)
{
	QMouseEvent* mouseE = dynamic_cast<QMouseEvent*>(e);
	if(mouseE){
		if(mouseE->type() == QEvent::MouseMove){
			if(playGui.handleMouseMove(mouseE))
				return true;
			if(!dialogMode()){
				QPointF mousePos = mouseE->localPos();
				QVector3D source = _player.camera().position();
				QVector3D target = _player.camera().frontVector();
				source += QVector3D(mousePos.x()*_map.caseSize.width()*-target.z(),
									-mousePos.y()*_map.caseSize.height()*0.5,
									mousePos.x()*_map.caseSize.width()*target.x());
				target += QVector3D(mousePos.x()*0.2*-target.z(),
									-mousePos.y()*0.2,
									mousePos.x()*0.2*target.x());
				if(_map.handleMouseRay(source, target))
					return true;
			}
		}
		else if(mouseE->type() == QEvent::MouseButtonPress){
			return playGui.handleMousePress(mouseE);
		}
		else if(mouseE->type() == QEvent::MouseButtonRelease){
			if(playGui.handleMouseRelease(mouseE))
					return true;
			if(_map.handleMouseClick(_player))
					return true;
		}
	}
	if(!dialogMode())
		return _player.inputHandle(game,e);
	return false;
}

void Level::update(Game & game)
{
	if(_player.life()<=0){
		displayGameOver();
	}
	if(!dialogMode()){
		t+=tInc;
		if(t>1.f){
			t=1.f;
			tInc*=-1;
			transformLight = transformLight.inverted();
		}
		else if(t<0.f){
			t=0.f;
			tInc*=-1;
			transformLight = transformLight.inverted();
		}

		dirLight.direction = (transformLight * QVector4D(dirLight.direction)).toVector3D();
		_player.update(game);
		playGui.update();

		_map.update();
	}
}

void Level::render(Game & game) const
{
	Renderer& renderer = game.renderer();
	renderer.initRender();

	glEnable(GL_DEPTH_TEST);
	renderer.projectionType = Renderer::Perspective;
	game.updateProjection();
	renderer.bindShader(Renderer::Normal);
	renderer.bindCamera(_player.camera());
	renderer.bindDirectionalLight(dirLight);
	renderer.bindTorchLight(torchLight);
	renderer.draw(objects);
	_map.draw(renderer);

	playGui.render(game);

	models[Beer2D].first()->transformM.setToIdentity();
	models[Beer2D].first()->transformM.translate(0.75f,-0.75f,0);
	for(int i = 0; i<_player.duffCount(); i++){
		renderer.draw(models[Beer2D]);
		models[Beer2D].first()->transformM.translate(-0.07f,0,0);
	}
	models[Donut2D].first()->transformM.setToIdentity();
	models[Donut2D].first()->transformM.translate(0.75f,-0.85f,0);
	for(int i = 0; i<_player.life(); i++){
		renderer.draw(models[Donut2D]);
		models[Donut2D].first()->transformM.translate(-0.07f,0,0);
	}

	renderer.endRender();
}

void Level::clear(Game & game)
{
	Renderer& renderer = game.renderer();
	AudioPlayer* player = AudioManager::player("apu.mp3");
	if(player)
		player->stop();
	renderer.clearVAO();
	renderer.clearVBO();
	renderer.clearTextures();
	objects.clear();
	for(int i=0;i<models.size();i++){
		models[i].clear();
	}
	playGui.clear();
	_map.clear();
	_player.clear();
	hideDialog();
}

void Level::askPreviousMode()
{
	previousDialog.show();
}

void Level::askNextMode()
{
	nextDialog.show();
}

bool Level::testCondition(const Player &p)
{
	return p.duffCount() >= _condition;
}

void Level::displayCondition()
{
	playGui.appendTmpElement(120,conditionLbl);
}

void Level::displayWallMsg()
{
	playGui.appendTmpElement(80,wallLbl);
}

void Level::displayDoorMsg()
{
	playGui.appendTmpElement(80,doorInstructionLbl);
}

void Level::displayGameOver()
{
	gameOverDialog.showOkDialog();
}

const Map &Level::map()
{
	return _map;
}

Player &Level::player()
{
	return _player;
}

void Level::hideDialog()
{
	nextDialog.hide();
	previousDialog.hide();
	exitDialog.hide();
	gameOverDialog.hide();
}

bool Level::dialogMode() const
{
	return nextDialog.isVisible() ||
			previousDialog.isVisible() ||
			exitDialog.isVisible() ||
			gameOverDialog.isVisible();
}

int Level::condition() const
{
	return _condition;
}
