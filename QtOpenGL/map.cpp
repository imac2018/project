#include "map.h"
#include "level.h"
#include "player.h"
#include "game.h"
#include <iostream>

void Map::loadCase(int x, int y, QColor c)
{
	if(c == Qt::white){
		cases[x][y] = new EmptyCase(x,y);
		return;
	}
	if(c == Qt::black){
		cases[x][y] = new WallCase(x,y);
		return;
	}
	if(c.red()==255){
		entry = new PreviousCase(x,y);
		cases[x][y] = entry;
		switch (c.blue()) {
		case 0:
		case 1:
		case 2:
		case 3:
			_initialOrientation = (Orientation)c.blue();
			break;
		default:
			_initialOrientation = NORTH;
			break;
		}
		return;
	}
	if(c.green()==255){
		exit = new NextCase(x,y);
		cases[x][y] = exit;
		return;
	}
	if(c.red() == 170 && c.green() == 119){
		Orientation doorDir;
		switch (c.blue()) {
		case 34:
		case 35:
		case 36:
		case 37:
			doorDir = (Orientation)(c.blue()-34);
			break;
		default:
			doorDir = NORTH;
			break;
		}
		cases[x][y] = new DoorCase(doorDir,x,y);
		return;
	}
	if(c.blue()==255){
		cases[x][y] = new WaterCase(x,y);
		return;
	}
	cases[x][y] = NULL;
}

void Map::loadMap(const QString &filepath)
{
	QImage map(filepath);
	width = map.width();
	height = map.height();

	this->cases = new Case**[width];
	for(int i = 0; i<width; i++){
		cases[i] = new Case*[height];
		for(int j = 0; j<height; j++){
			QColor pixel = map.pixel(i,j);
			loadCase(i,j,pixel);
		}
	}
	for(int i = 0; i<width; i++){
		for(int j = 0; j<height; j++){
			std::cout << cases[i][j]->type() << " ";
		}
		std::cout << std::endl;
	}

}

float Map::initializeMapModels(Game &game)
{
	Renderer& renderer = game.renderer();
	if(initialisationState<0.05f){
		models[GroundDefault] = new Object3D(renderer.makeTexturedFace(90, 0,0,0,0,0,
																	   caseSize.width()*0.5,2));
		models[GroundDefault]->texture= renderer.addTexture(":/assets/sol_default.png");
		initialisationState = 0.05f;
		return initialisationState;
	}
	if(initialisationState<0.075f){
		models[Ground2] = new Object3D(renderer.makeTexturedFace(90, 0,0,0,0,0,
																 caseSize.width()*0.5,2));
		models[Ground2]->texture= renderer.addTexture(":/assets/sol_2.png");
		initialisationState = 0.075f;
		return initialisationState;
	}
	if(initialisationState<0.1f){
		models[Ground3] = new Object3D(renderer.makeTexturedFace(90, 0,0, 0, 0, 0,
																 caseSize.width()*0.5,2));
		models[Ground3]->texture= renderer.addTexture(":/assets/sol_3.png");
		initialisationState = 0.1f;
		return initialisationState;
	}
	if(initialisationState<0.12f){
		models[CeilingDefault] = new Object3D(renderer.makeTexturedFace(90, 0,0, 0, 0, 0,
																 caseSize.width()*0.5,3));
		models[CeilingDefault]->texture= renderer.addTexture(":/assets/plafond_default.png");
		initialisationState = 0.12f;
		return initialisationState;
	}
	if(initialisationState<0.15f){
		models[Ceiling2] = new Object3D(renderer.makeTexturedFace(90, 0,0, 0, 0, 0,
																 caseSize.width()*0.5,3));
		models[Ceiling2]->texture= renderer.addTexture(":/assets/plafond_2.png");
		initialisationState = 0.15f;
		return initialisationState;
	}
	if(initialisationState<0.2f){
		models[Ground4] = new Object3D(renderer.makeTexturedFace(90, 0,0, 0,0,0,
																 caseSize.width()*0.5));
		models[Ground4]->texture= renderer.addTexture(":/assets/sol_4.png");
		initialisationState = 0.2f;
		return initialisationState;
	}
	if(initialisationState<0.22f){
		models[WallFace3] = new Object3D(renderer.makeTexturedFace(0, 0,0,0,0,0,caseSize.width()*0.5));
		//models[WallFace]->texture= renderer.addTexture(":/assets/sol_4.png");
		initialisationState = 0.22f;
		return initialisationState;
	}
	if(initialisationState<0.24f){
		models[WallFaceDefault] = new Object3D(renderer.makeTexturedFace(0, 0,0,0,0,0,caseSize.width()*0.5));
		//models[WallFace]->texture= renderer.addTexture(":/assets/sol_4.png");
		initialisationState = 0.24f;
		return initialisationState;
	}
	if(initialisationState<0.25f){
		models[WallFace2] = new Object3D(renderer.makeTexturedFace(0, 0,0,0,0,0,caseSize.width()*0.5));
		//models[WallFace]->texture= renderer.addTexture(":/assets/sol_4.png");
		initialisationState = 0.25f;
		return initialisationState;
	}
	if(initialisationState<0.27f){
		models[WallSideDefault] = new Object3D(renderer.makeTexturedFace(0, 90,0,0,0,0,caseSize.width()*0.5));
		//models[WallFace]->texture= renderer.addTexture(":/assets/sol_4.png");
		initialisationState = 0.27f;
		return initialisationState;
	}
	if(initialisationState<0.28f){
		models[WallSide2] = new Object3D(renderer.makeTexturedFace(0, 90,0,0,0,0,caseSize.width()*0.5));
		//models[WallFace]->texture= renderer.addTexture(":/assets/sol_4.png");
		initialisationState = 0.28f;
		return initialisationState;
	}
	if(initialisationState<0.3f){
		models[WallSide3] = new Object3D(renderer.makeTexturedFace(0, 90,0,0,0,0,caseSize.width()*0.5));
		//models[WallFace]->texture= renderer.addTexture(":/assets/sol_4.png");
		for(int i=0;i<models.size();i++){
			if(models[i]){
				models[i]->diffuseColor = QColor(120,120,130);
				models[i]->specularColor = QColor(20,20,40);
			}
		}
		initialisationState = 0.3f;
		return initialisationState;
	}
	return initialisationState;

}

float Map::initializeMapObjects(Game &game)
{
	int end = qMin(icase+15, width*height);
	Object3D* o;
	QVector3D pos;
	int x,y;
	for(;icase<end;icase++){
		x = icase/height;
		y = icase%height;
		if(cases[x][y]==NULL)
			continue;
		if(cases[x][y]->type() == Case::Empty){
			int type = qrand()%15;
			if(type>Ground4 || type<=GroundDefault)
				o = new Object3D(*models[GroundDefault]);
			else
				o = new Object3D(*models[type]);
			pos = caseCenter(x,y);
			o->transformM.translate(pos);
			objects.append(o);
			addWalls(game, x,y);
		}

	}

	if(icase>=width*height){
		makeCeiling(game);
		initialisationState = 1;
	}
	return initialisationState;
}

void Map::addWalls(Game &game, int x, int y)
{
	Object3D* o;
	QVector3D pos;
	if(x+1<width){
		if(cases[x+1][y]->type() == Case::Wall){
			int type = qrand()%10;
			if(type>WallSide3 || type<=WallSideDefault)
				o = new Object3D(*models[WallSideDefault]);
			else
				o = new Object3D(*models[type]);
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(caseSize.width()*0.5,0,0));
			objects.append(o);
		}
	}
	if(x-1>=0){
		if(cases[x-1][y]->type() == Case::Wall){
			int type = qrand()%10;
			if(type>WallSide3 || type<=WallSideDefault)
				o = new Object3D(*models[WallSideDefault]);
			else
				o = new Object3D(*models[type]);
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(caseSize.width()*-0.5,0,0));
			objects.append(o);
		}
	}
	if(y+1<height){
		if(cases[x][y+1]->type() == Case::Wall){
			int type = qrand()%10;
			if(type>WallFace3 || type<=WallFaceDefault)
				o = new Object3D(*models[WallFaceDefault]);
			else
				o = new Object3D(*models[type]);
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(0,0,caseSize.height()*0.5));
			objects.append(o);
		}
	}
	if(y-1>=0){
		if(cases[x][y-1]->type() == Case::Wall){
			int type = qrand()%10;
			if(type>WallFace3 || type<=WallFaceDefault)
				o = new Object3D(*models[WallFaceDefault]);
			else
				o = new Object3D(*models[type]);
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(0,0,caseSize.height()*-0.5));
			objects.append(o);
		}
	}
}

void Map::makeCeiling(Game &game)
{
	QVector3D pos;
	Object3D* o;
	for(int i = 0; i<=width*0.1;i++){
		for(int j = 0; j<=height*0.1;j++){
			int type = qrand()%3;
			if(type)
				o = new Object3D(*models[CeilingDefault]);
			else
				o = new Object3D(*models[Ceiling2]);
			o->transformM.translate(QVector3D(i*caseSize.width()*10,
												  caseSize.width()*1.5,
											   -j*caseSize.height()*10));
			o->transformM.scale(10,0,10);
			objects.append(o);
		}

	}
}

Map::Map(const QString &filepath, Level &level)
	: entry(NULL), exit(NULL), caseSize(15.f,15.f),
	  parent(level), models(ModelTypeCount),
	  initialisationState(0), icase(0)
{
	loadMap(filepath);
}



float Map::initialize(Game &game)
{
	if(initialisationState<0.3)
		return initializeMapModels(game);
	return initializeMapObjects(game);
}

QVector3D Map::caseCenter(int x, int y) const
{
	return QVector3D(x * caseSize.width(), -1, -(height - 1 - y) * caseSize.height());
}

void Map::moveForward(Player &p)
{
	QPoint newPos = p.mapPosition() + Map::getMoveFromOrientation(p.direction());
	if(newPos.x()>=0 && newPos.x()<width
			&& newPos.y()>=0 && newPos.y()<height){
		if(cases[newPos.x()][newPos.y()]->isReachable(p,parent)){
			p.moveForward(cases[newPos.x()][newPos.y()]);
		}
	}
}

void Map::moveBack(Player &p)
{
	QPoint newPos = p.mapPosition() - Map::getMoveFromOrientation(p.direction());
	if(newPos.x()>=0 && newPos.x()<width
			&& newPos.y()>=0 && newPos.y()<height){
		if(cases[newPos.x()][newPos.y()]->isReachable(p,parent)){
			p.moveBack(cases[newPos.x()][newPos.y()]);
		}
	}
}

void Map::draw(Renderer& renderer) const
{
	renderer.draw(objects);
}

QPoint Map::initialMapPosition() const
{
	return entry->pos();
}

QVector3D Map::initialRealPosition() const
{
	return caseCenter(entry->pos().x(),entry->pos().y());
}

Orientation Map::initialOrientation() const
{
	return _initialOrientation;
}

QPoint Map::getMoveFromOrientation(Orientation o)
{
	switch(o){
	case NORTH:
		return QPoint(0,-1);
	case SOUTH:
		return QPoint(0,1);
	case EAST:
		return QPoint(1,0);
	case WEST:
		return QPoint(-1,0);
	}
	return QPoint(0,0);
}

Case::Case(Case::Type type, int x, int y)
	: _type(type), _pos(x,y)
{}

Case::Type Case::type() const
{
	return _type;
}

QPoint Case::pos() const
{
	return _pos;
}

PreviousCase::PreviousCase(int x, int y)
	: Case(Previous, x, y)
{}

bool PreviousCase::isReachable(const Player &, Level &) const
{
	return true;
}

void PreviousCase::action(Player &p, Level &l)
{
	l.askPreviousMode();
}

NextCase::NextCase(int x, int y)
	: Case(Next, x, y)
{
}

bool NextCase::isReachable(const Player &, Level &l) const
{
	return true;
}

void NextCase::action(Player &p, Level &l)
{
	l.askNextMode();
}

EmptyCase::EmptyCase(int x, int y)
	: Case(Empty, x, y)
{}

bool EmptyCase::isReachable(const Player &, Level &) const
{
	return true;
}

void EmptyCase::action(Player &, Level &)
{}

DoorCase::DoorCase(Orientation direction, int x, int y)
	:	Case(Door,x, y), direction(direction), locked(true)
{}

bool DoorCase::isReachable(const Player &p, Level &l) const
{
	return !locked;
}

void DoorCase::action(Player &p, Level &l)
{}

WaterCase::WaterCase(int x, int y)
	: Case(Water,x,y)
{}

bool WaterCase::isReachable(const Player &, Level &) const
{
	return false;
}

void WaterCase::action(Player &, Level &)
{}

WallCase::WallCase(int x, int y)
	: Case(Wall,x,y)
{}

bool WallCase::isReachable(const Player &p, Level &l) const
{
	return false;
}

void WallCase::action(Player &p, Level &l)
{}
