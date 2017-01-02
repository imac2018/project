#include "map.h"
#include "level.h"
#include "player.h"
#include "game.h"
#include "mapelement.h"
#include "item.h"
#include "enemy.h"
#include "audio.h"
#include <iostream>

class DoorElement : public MapElement{
	DoorCase* parent;

public:
	DoorElement(DoorCase* parent,
				QList<Object3D*> models,
				Collider* collider);
	void actionOnCollision(Player &p);
	bool isDead();
	bool blockPath() const;

	// MapElement interface
public:
	void actionOnClick(Player &p);
	void unhighlight();
};

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
		switch (c.blue()) {
		case 0:
		case 1:
		case 2:
		case 3:
			exit = new NextCase(x,y,(Orientation)c.blue());
			break;
		default:
			exit = new NextCase(x,y,EAST);
			break;
		}

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
		models[Ground].append(new Object3D(renderer.makeTexturedFace(90, 0,0,
																0,0,0,
																caseSize.width()*0.5,
																caseSize.height()*0.5,2)));
		textures[T_Ground1]= renderer.addTexture(":/assets/images/sol_default.png");
		initialisationState = 0.05f;
		return initialisationState;
	}
	if(initialisationState<0.075f){
		textures[T_Ground2]= renderer.addTexture(":/assets/images/sol_2.png");
		initialisationState = 0.075f;
		return initialisationState;
	}
	if(initialisationState<0.1f){
		textures[T_Ground3]= renderer.addTexture(":/assets/images/sol_3.png");
		initialisationState = 0.1f;
		return initialisationState;
	}
	if(initialisationState<0.12f){
		models[Ceiling].append(new Object3D(renderer.makeTexturedFace(-90, 0,0, 0, 0, 0,
																 caseSize.width()*0.5,
																 caseSize.height()*0.5,3)));
		textures[T_Ceiling1]= renderer.addTexture(":/assets/images/plafond_default.png");
		initialisationState = 0.12f;
		return initialisationState;
	}
	if(initialisationState<0.13f){
		textures[T_Comptoir]= renderer.addTexture(":/assets/images/comptoir.png");
		initialisationState = 0.13f;
		return initialisationState;
	}
	if(initialisationState<0.15f){
		textures[T_Ceiling2]= renderer.addTexture(":/assets/images/plafond_2.png");
		initialisationState = 0.15f;
		return initialisationState;
	}
	if(initialisationState<0.2f){
		textures[T_Ground4]= renderer.addTexture(":/assets/images/sol_4.png");
		initialisationState = 0.2f;
		return initialisationState;
	}
	if(initialisationState<0.22f){
		models[WallForward].append(new Object3D(renderer.makeTexturedFace(0,0,0,
																	 0,caseSize.width()*0.5,0,
																	 caseSize.width()*0.5)));
		textures[T_Wall1]=renderer.addTexture(":/assets/images/rayon_1.png");
		initialisationState = 0.22f;
		return initialisationState;
	}
	if(initialisationState<0.24f){
		models[WallBackward].append(new Object3D(renderer.makeTexturedFace(0,180,0,
																	  0,caseSize.width()*0.5,0,
																	  caseSize.width()*0.5)));
		textures[T_Wall0]=renderer.addTexture(":/assets/images/mursimple.png");
		initialisationState = 0.24f;
		return initialisationState;
	}
	if(initialisationState<0.25f){
		models[WallLeft].append(new Object3D(renderer.makeTexturedFace(0, -90,0,
																  0,caseSize.width()*0.5,0,
																  caseSize.width()*0.5)));
		textures[T_Wall2]=renderer.addTexture(":/assets/images/rayon_2.png");
		initialisationState = 0.25f;
		return initialisationState;
	}
	if(initialisationState<0.26f){
		textures[T_Wall4]=renderer.addTexture(":/assets/images/rayon_4.png");
		initialisationState = 0.26f;
		return initialisationState;
	}
	if(initialisationState<0.27f){
		models[WallRight].append(new Object3D(renderer.makeTexturedFace(0, 90,0,
																   0,caseSize.width()*0.5,0,
																   caseSize.width()*0.5)));
		textures[T_Wall3]=renderer.addTexture(":/assets/images/rayon_3.png");
		initialisationState = 0.27f;
		return initialisationState;
	}
	if(initialisationState<0.275f){
		QList<Object3D*> objects = Object3D::importFromFile(":/models/porte.obj");
		if(objects.length()){
			foreach(Object3D* o, objects){
				o->rotate(0,90,0);
				o->scale(1.5);
				o->translate(0,-3,0);
			}
			models[Door] = objects;
		}

		initialisationState = 0.275f;
		return initialisationState;
	}
	if(initialisationState<0.29f){
		QList<Object3D*> objects = Object3D::importFromFile(":/models/contour.obj");
		if(objects.length()){
			foreach(Object3D* o, objects){
				o->rotate(0,90,0);
				o->scale(1.5f);
				o->translate(2.1,0,0.5);
			}
			models[DoorBorder].append(objects);
		}

		initialisationState = 0.29f;
		return initialisationState;
	}
	if(initialisationState<0.3f){
		foreach(Object3D* o, models[Door]){
			renderer.addObject(*o);
		}
		foreach(Object3D* o, models[DoorBorder]){
			renderer.addObject(*o);
		}
		models[DoorBorder].append(new Object3D(renderer.makeTexturedFace(0, 0,0,
																   -caseSize.width()*0.5,caseSize.width()*0.5,0,
																   caseSize.width()*0.25,caseSize.width()*0.5)));
		models[DoorBorder].last()->texture = textures[T_Wall0];
		models[DoorBorder].append(new Object3D(renderer.makeTexturedFace(0, 0,0,
																   0,caseSize.width()*0.93,0,
																   caseSize.width(),caseSize.width()*0.06)));
		models[DoorBorder].last()->texture = textures[T_Wall0];
		models[DoorBorder].append(new Object3D(renderer.makeTexturedFace(0, 0,0,
																   caseSize.width()*0.5,caseSize.width()*0.5,0,
																   caseSize.width()*0.38,caseSize.width()*0.5)));
		models[DoorBorder].last()->texture = textures[T_Wall0];
		initialisationState = 0.3f;
		return initialisationState;
	}
	if(initialisationState<0.32f){
		QList<Object3D*> objects = Object3D::importFromFile(":/models/stairs4.3DS");
		if(objects.length()){
			//QOpenGLTexture* texture = renderer.addTexture(":/assets/images/cardboardbox.jpg");
			foreach(Object3D* o, objects){
				//o->texture = texture;
				o->rotate(180,0,180);
				o->scale(caseSize.width()*0.85,caseSize.width()*0.5,caseSize.width()*0.3);
				o->translate(caseSize.width(),-caseSize.width()*0.45,caseSize.width()*0.05);

			}
			models[Stairs] = objects;
		}
		initialisationState = 0.32f;
		return initialisationState;
	}
	if(initialisationState<0.35f){
		QList<Object3D*> objects = Object3D::importFromFile(":/models/cardboardbox.obj");
		if(objects.length()){
			QOpenGLTexture* texture = renderer.addTexture(":/assets/images/cardboardbox.jpg");
			foreach(Object3D* o, objects){
				//o->rotate(70,50,25);
				o->translate(0,0.5,0);
				o->scale(2);
				o->texture = texture;

			}
			models[Water] = objects;
		}
		initialisationState = 0.35f;
		return initialisationState;
	}
	if(initialisationState<0.4f){
		foreach(Object3D* o, models[Water]){
			renderer.addObject(*o);
		}
		foreach(Object3D* o, models[Stairs]){
			renderer.addObject(*o);
		}
		for(int i=0;i<models.size();i++){
			if(models[i].length()){
				foreach(Object3D* o, models[i]){
					o->diffuseColor = QColor(150,150,1550);
					o->specularColor = QColor(40,40,60);
				}
			}
		}
		initialisationState = 0.4f;
		return initialisationState;
	}
	return initialisationState;

}

float Map::initializeMapObjects()
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
		if(cases[x][y]->type() == Case::Previous){
			pos = caseCenter(x,y);
			foreach(Object3D* o, models[Stairs]){
				o = new Object3D(*o);
				o->transformM.translate(pos);
				QMatrix4x4 rotater;
				switch (_initialOrientation) {
				case EAST:
					rotater.rotate(0,90,0);
				case WEST:
					rotater.rotate(0,270,0);
					break;
				case SOUTH:
					rotater.rotate(0,180,0);;
					break;
				}
				o->transformM = o->transformM*rotater;

				objects.append(o);
			}
			addWalls(x,y);
			continue;
		}
		if(cases[x][y]->type() != Case::Wall){
			int type = qrand()%15;
			o = new Object3D(*models[Ground].first());
			if(type>T_Ground4 || type<T_Ground1)
				o->texture = textures[T_Ground1];
			else
				o->texture = textures[type];
			pos = caseCenter(x,y);
			o->transformM.translate(pos);
			objects.append(o);
			addWalls(x,y);
			if(cases[x][y]->type() == Case::Door) {
				foreach(Object3D* model, models[DoorBorder]){
					o = new Object3D(*model);
					o->transformM.translate(pos);
					objects.append(o);
				}
				DoorElement* door = new DoorElement(dynamic_cast<DoorCase*>(cases[x][y]),models[Door],
													new ColliderCube(QVector3D(0,-1,0),
																	 QVector3D(-caseSize.width()*0.5,0,0),
																	 QVector3D(0,caseSize.height()*0.5,0),
																	 QVector3D(0,0,2)));
				placeElement(door,x,y);
				QMatrix4x4 mat;
				mat.translate(2,0,0.5);
				door->transform(mat);
			}
			else if(cases[x][y]->type() == Case::Water){
				foreach(Object3D* model, models[Water]){
					o = new Object3D(*model);
					o->transformM.translate(pos);
					objects.append(o);
				}
			}
			else if(cases[x][y]->type() == Case::Next){
				switch (dynamic_cast<NextCase*>(cases[x][y])->direction) {
				case EAST:
				case WEST:
					o = new Object3D(*models[WallRight].first());
					o->texture = textures[T_Comptoir];
					pos = caseCenter(x,y);
					o->transformM.translate(pos);
					objects.append(o);
					break;
				default:
					o = new Object3D(*models[WallForward].first());
					o->texture = textures[T_Comptoir];
					pos = caseCenter(x,y);
					o->transformM.translate(pos);
					objects.append(o);
					break;
				}
			}
		}


	}

	if(icase>=width*height){
		makeCeiling();
		makeDeepWall();
		initialisationState = 1;
	}
	return initialisationState;
}

void Map::addWalls(int x, int y)
{
	Object3D* o;
	QVector3D pos;
	if(x+1<width){
		if(cases[x+1][y]->type() == Case::Wall){
			int type = qrand()%10;
			o = new Object3D(*models[WallRight].first());
			if(type<T_Wall0 || type>T_Wall4)
				o->texture = textures[T_Wall0];
			else
				o->texture = textures[type];
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(caseSize.width()*0.5,0,0));
			objects.append(o);
		}
	}
	if(x-1>=0){
		if(cases[x-1][y]->type() == Case::Wall){
			int type = qrand()%10;
			o = new Object3D(*models[WallLeft].first());
			if(type<T_Wall0 || type>T_Wall4)
				o->texture = textures[T_Wall0];
			else
				o->texture = textures[type];
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(caseSize.width()*-0.5,0,0));
			objects.append(o);
		}
	}
	if(y+1<height){
		if(cases[x][y+1]->type() == Case::Wall){
			int type = qrand()%10;
			o = new Object3D(*models[WallForward].first());
			if(type<T_Wall0 || type>T_Wall4)
				o->texture = textures[T_Wall0];
			else
				o->texture = textures[type];
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(0,0,caseSize.height()*0.5));
			objects.append(o);
		}
	}
	if(y-1>=0){
		if(cases[x][y-1]->type() == Case::Wall){
			int type = qrand()%10;
			o = new Object3D(*models[WallBackward].first());
			if(type<T_Wall0 || type>T_Wall4)
				o->texture = textures[T_Wall0];
			else
				o->texture = textures[type];
			pos = caseCenter(x,y);
			o->transformM.translate(pos+QVector3D(0,0,caseSize.height()*-0.5));
			objects.append(o);
		}
	}
}

void Map::makeCeiling()
{
	Object3D* o;
	for(int i = 0; i<=width*0.1;i++){
		for(int j = 0; j<=height*0.1;j++){
			int type = qrand()%10;
			o = new Object3D(*models[Ceiling].first());
			if(type>T_Ceiling2 || type<T_Ceiling1)
				o->texture = textures[T_Ceiling1];
			else
				o->texture = textures[type];
			o->transformM.translate(QVector3D(i*caseSize.width()*10,
												  caseSize.width()*1.5,
											   -j*caseSize.height()*10));
			o->transformM.scale(10,0,10);
			objects.append(o);
		}

	}
}

void Map::makeDeepWall()
{
	Object3D* o = new Object3D(*models[WallForward].first());
	o->texture = textures[T_Wall0];
	QVector3D pos = caseCenter(width*0.5,height-1);
	o->transformM.translate(pos);
	o->transformM.scale(caseSize.width()*(width+1));
	objects.append(o);
	o = new Object3D(*models[WallBackward].first());
	o->texture = textures[T_Wall0];
	pos = caseCenter(width*0.5,0);
	o->transformM.translate(pos);
	o->transformM.scale(caseSize.width()*(width+1));
	objects.append(o);
	o = new Object3D(*models[WallRight].first());
	o->texture = textures[T_Wall0];
	pos = caseCenter(0,height*0.5);
	o->transformM.translate(pos);
	o->transformM.scale(caseSize.width()*(width+1));
	objects.append(o);
	o = new Object3D(*models[WallLeft].first());
	o->texture = textures[T_Wall0];
	pos = caseCenter(width-1,height*0.5);
	o->transformM.translate(pos);
	o->transformM.scale(caseSize.width()*(width+1));
	objects.append(o);
}

void Map::clear()
{
	for(int i=0;i<models.size();i++){
		models[i].clear();
	}
	objects.clear();
	elements.clear();
	icase = 0;
	initialisationState = 0;
	hoveredElement = NULL;
}

Map::Map(Level &level)
	: entry(NULL), exit(NULL), caseSize(15.f,15.f),
	  parent(level), models(ModelTypeCount), textures(TextureTypeCount),
	  initialisationState(0), icase(0), hoveredElement(NULL)
{}



float Map::initialize(Game &game)
{
	if(initialisationState<0.4)
		return initializeMapModels(game);
	return initializeMapObjects();
}

QVector3D Map::caseCenter(int x, int y) const
{
	return QVector3D(x * caseSize.width(), -1, -(height - 1 - y) * caseSize.height());
}

const Case &Map::getCase(int x, int y) const
{
	return *(cases[x][y]);
}

const Case &Map::getCase(QPoint p) const
{
	return getCase(p.x(),p.y());
}

void Map::placeElement(MapElement *e, int x, int y)
{
	Enemy* enemy = dynamic_cast<Enemy*>(e);
	if(!enemy){
		e->resetPosition();
		QMatrix4x4 transform;
		transform.translate(caseCenter(x,y));
		transform.translate(0,3,0);
		e->transform(transform);
		e->mapPos = QPoint(x,y);
	}
	else{
		enemy->setPosition(QPoint(x,y),caseCenter(x,y));
	}
	elements.append(e);
}

void Map::moveForward(Player &p)
{
	QPoint newPos = p.mapPosition() + Map::getMoveFromOrientation(p.direction());
	if(newPos.x()>=0 && newPos.x()<width
			&& newPos.y()>=0 && newPos.y()<height){
		if(cases[newPos.x()][newPos.y()]->isReachable(p,parent)){
			foreach(MapElement* e, elements){
				if(e->mapPos == newPos){
					if(e->blockPath()){
						return;
					}
					e->actionOnCollision(p);
				}
			}
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

bool Map::handleMouseRay(QVector3D source, QVector3D direction)
{
	if(hoveredElement){
		if(hoveredElement->intersectRay(source, direction)){
			return true;
		}
		hoveredElement->unhighlight();
		hoveredElement = NULL;
	}
	float distance = 0, tmpDistance;
	foreach (MapElement* e, elements) {
		if(e->intersectRay(source, direction)){
			if(hoveredElement==NULL){
				hoveredElement = e;
				distance = (e->position() - source).lengthSquared();
			}
			else{
				tmpDistance =  (e->position() - source).lengthSquared();
				if(tmpDistance<distance){
					distance = tmpDistance;
					hoveredElement = e;
				}
			}
		}
	}
	if(distance>0.01f){
		hoveredElement->highlight();
		return true;
	}
	return false;
}

bool Map::handleMouseClick(Player& p)
{
	if(hoveredElement){
		hoveredElement->actionOnClick(p);
		return true;
	}
	return false;
}

void Map::draw(Renderer& renderer) const
{
	renderer.draw(objects);
	foreach(MapElement* e, elements){
		renderer.draw(e->objects());
	}
}

void Map::update()
{
	foreach(MapElement* element, elements){
		element->update(parent);
	}
	QMutableListIterator<MapElement*> iremover(elements);
	while(iremover.hasNext()){
		if(iremover.next()->isDead())
			iremover.remove();
	}
	if(hoveredElement)
		if(hoveredElement->isDead())
			hoveredElement = NULL;
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

bool PreviousCase::isReachable() const
{
	return false;
}

NextCase::NextCase(int x, int y, Orientation direction)
	: Case(Next, x, y), direction(direction)
{
}

bool NextCase::isReachable(const Player & p, Level &l) const
{
	if(l.testCondition(p)){
		return true;
	}
	l.displayCondition();
	return false;
}

void NextCase::action(Player &, Level &l)
{
	l.askNextMode();
}

bool NextCase::isReachable() const
{
	return false;
}

EmptyCase::EmptyCase(int x, int y)
	: Case(Empty, x, y)
{}

bool EmptyCase::isReachable(const Player &, Level &) const
{
	return isReachable();
}

void EmptyCase::action(Player &p, Level &)
{}

bool EmptyCase::isReachable() const
{
	return true;
}

DoorCase::DoorCase(Orientation direction, int x, int y)
	:	Case(Door,x, y), direction(direction), locked(true)
{}

bool DoorCase::isReachable(const Player &p, Level &l) const
{
	if(locked){
		l.displayDoorMsg();
	}
	return isReachable();
}

void DoorCase::action(Player &p, Level &l)
{}

bool DoorCase::isReachable() const
{
	return !locked;
}

WaterCase::WaterCase(int x, int y)
	: Case(Water,x,y)
{}

bool WaterCase::isReachable(const Player &, Level &) const
{
	return false;
}

void WaterCase::action(Player &, Level &)
{}

bool WaterCase::isReachable() const
{
	return false;
}

WallCase::WallCase(int x, int y)
	: Case(Wall,x,y)
{}

bool WallCase::isReachable(const Player &p, Level &l) const
{
	AudioPlayer* player = AudioManager::player("d'oh.mp3");
	if(player){
		player->setVolume(95);
		player->play();
	}
	l.displayWallMsg();
	return false;
}

void WallCase::action(Player &p, Level &l)
{}

bool WallCase::isReachable() const
{
	return false;
}

DoorElement::DoorElement(DoorCase *parent, QList<Object3D *> models, Collider *collider)
	:	MapElement(models,collider), parent(parent)
{
}

void DoorElement::actionOnCollision(Player &p)
{}

bool DoorElement::isDead()
{
	return false;
}

void DoorElement::unhighlight(){
	foreach (Object3D* o, _objects) {
		o->ambiantColor = Qt::gray;
		o->diffuseColor = Qt::darkGray;
		o->specularColor = Qt::darkGray;
	}
}

bool DoorElement::blockPath() const
{
	return parent->locked;
}

void DoorElement::actionOnClick(Player &p)
{
	parent->locked = !parent->locked;
	QMatrix4x4 rotation;
	rotation.rotate(parent->locked?90:-90,0,1,0);
	transformRight(rotation);
}
