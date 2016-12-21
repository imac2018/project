#include "game.h"
#include "exception"
#include "configdialog.h"
#include <QApplication>

QVector<Character*> Game::characters;
QVector<Player> Game::defaultPlayers;
QMap<QString,Ship*> Game::shipModels;
QMap<QString,Weapon*> Game::weaponModels;
QVector<QVector<QPair<Input, ShipAction> > > Game::defaultInputs;
Qt::Key Game::breakKey = Qt::Key_P;
Qt::Key Game::configKey = Qt::Key_Escape;

Game::Game()
	: next(NULL), currentLevelFile(":/levels/level1.xml")
{
	staticInit();
	addDefaultPlayer(true);
	addDefaultPlayer(false);
	addDefaultPlayer(false);
	configDialog = new ConfigDialog();
	current = new Galata(QFile(currentLevelFile), players.first());

	QObject::connect(configDialog, SIGNAL(accepted()),
					 this,SLOT(setConfig()));
	QObject::connect(configDialog, SIGNAL(refused()),
					 configDialog,SLOT(close()));
	QObject::connect(configDialog, SIGNAL(exitAsked()),
					 QApplication::instance(),SLOT(quit()));
}



void Game::staticInit()
{
	if(characters.empty()){

	}

	if(defaultPlayers.empty()){
		defaultPlayers.append(Player("Player1",QColor(217,35,42),3));
		defaultPlayers.append(Player("Player2",QColor(18,35,232),3));
		defaultPlayers.append(Player("Player3",QColor(180,175,22),3));
		defaultPlayers.append(Player("Player4",QColor(20,206,36),3));
	}

	if(shipModels.empty()){
		shipModels.insert("Falcon V", new Ship("Falcon V",QImage(":/images/falconVB.png"),
							   9,2,3,4,5,6));
		shipModels.insert("Wolf 45J", new Ship("Wolf 45J",QImage(":/images/wolf45J.png"),
							   6,5,5,4,5,6));
		shipModels.insert("Bettle 20R", new Ship("Bettle 20R",QImage(":/images/bettleR.png"),
							   7,7,7,0,0,0));
		shipModels.insert("Blue Bullet Bull", new Ship("Blue Bullet Bull",QImage(":/images/BlueBulletBull.png"),
							   3,10,10,4,5,6));
	}

	if(weaponModels.empty()){
		weaponModels.insert("Ray",new Weapon("Ray", QPointF(),{&Weapon::shootLaser1,
											 &Weapon::shootLaser2,
											 &Weapon::shootLaser3,
											 &Weapon::shootLaser4}));
		weaponModels.insert("Wave", new Weapon("Wave",QPointF(),{&Weapon::shootLaser1,
											 &Weapon::shootWave1,
											 &Weapon::shootWave2,
											 &Weapon::shootWave3}));

	}

	if(!defaultInputs.length()){
		defaultInputs.append({
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Down,Qt::NoModifier),
							&ShipActions::stopDown),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Up,Qt::NoModifier),
							&ShipActions::stopUp),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Left,Qt::NoModifier),
							&ShipActions::stopBackward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Right,Qt::NoModifier),
							&ShipActions::stopForward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier),
							&ShipActions::moveDown),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier),
							&ShipActions::moveUp),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier),
							&ShipActions::moveBackward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier),
							&ShipActions::moveForward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Space,Qt::NoModifier),
							&ShipActions::mainWeaponStop),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier),
							&ShipActions::mainWeaponActivate)
		});

		defaultInputs.append({
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_5,Qt::KeypadModifier),
							&ShipActions::stopDown),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_8,Qt::KeypadModifier),
							&ShipActions::stopUp),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_4,Qt::KeypadModifier),
							&ShipActions::stopBackward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_6,Qt::KeypadModifier),
							&ShipActions::stopForward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_5,Qt::KeypadModifier),
							&ShipActions::moveDown),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_8,Qt::KeypadModifier),
							&ShipActions::moveUp),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_4,Qt::KeypadModifier),
							&ShipActions::moveBackward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_6,Qt::KeypadModifier),
							&ShipActions::moveForward),
			 QPair<Input,ShipAction>(QMouseEvent(QEvent::MouseButtonRelease, QPointF(),
										Qt::LeftButton,Qt::NoButton,Qt::NoModifier),
							&ShipActions::mainWeaponStop),
			 QPair<Input,ShipAction>(QMouseEvent(QEvent::MouseButtonPress, QPointF(),
										Qt::LeftButton,Qt::NoButton,Qt::NoModifier),
							&ShipActions::mainWeaponActivate),
			 QPair<Input,ShipAction>(QMouseEvent(QEvent::MouseButtonPress, QPointF(),
										Qt::LeftButton,Qt::NoButton,Qt::KeypadModifier),
							&ShipActions::mainWeaponActivate),
			 QPair<Input,ShipAction>(QMouseEvent(QEvent::MouseButtonPress, QPointF(),
										Qt::LeftButton,Qt::NoButton,Qt::KeypadModifier),
							&ShipActions::mainWeaponActivate)
		});

		defaultInputs.append({
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_S,Qt::NoModifier),
							&ShipActions::stopDown),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Z,Qt::NoModifier),
							&ShipActions::stopUp),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Q,Qt::NoModifier),
							&ShipActions::stopBackward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_D,Qt::NoModifier),
							&ShipActions::stopForward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_S,Qt::NoModifier),
							&ShipActions::moveDown),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::NoModifier),
							&ShipActions::moveUp),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Q,Qt::NoModifier),
							&ShipActions::moveBackward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_D,Qt::NoModifier),
							&ShipActions::moveForward),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyRelease,Qt::Key_Control,Qt::NoModifier),
							&ShipActions::mainWeaponStop),
			 QPair<Input,ShipAction>(QKeyEvent(QEvent::KeyPress,Qt::Key_Control,Qt::NoModifier),
							&ShipActions::mainWeaponActivate)
		});

	}
}

void Game::reload()
{
	delete this->current;
	Galata* current = new Galata(QFile(currentLevelFile), players.first());
	for(int i=1;i<players.length();i++){
		if(players[i]->isInGame())
			current->addPlayer(players[i]);
	}
	this->current = current;
	emit modeHasChanged();
}

void Game::addDefaultPlayer(bool enabled)
{
	if(this->shipModels.empty() || this->weaponModels.empty())
		throw std::exception("The Game is not correctly initialized");

	if(this->players.length() < this->defaultPlayers.length()){
		Player p;
		Player* last;
		p = this->defaultPlayers[this->players.length()];

		this->players.append(new Player(p.name(),p.color(),p.life(),enabled));
		last = this->players.last();
		last->setShip(*(this->shipModels.first()));
		last->setMainWeapon(*(this->weaponModels.first()));
		setDefaultInputToPlayer(last);
	}
}

bool Game::setDefaultInputToPlayer(Player *p)
{
	int j;
	for(int i=0; i<defaultInputs.length();i++){
		j = 0;
		while(j<defaultInputs[i].length()){
			if(inputIsUsed(defaultInputs[i][j].first))
				break;
			j++;
		}
		if(j>=defaultInputs[i].length()){
			QPair<Input,ShipAction> control;
			foreach(control , defaultInputs[i]){
				p->addControl(control.first,control.second);
			}
			return true;
		}
	}
	return false;
}

bool Game::inputIsUsed(const Input &i) const
{
	if(i == QKeyEvent(QEvent::KeyRelease,
					  Game::breakKey,
					  Qt::NoModifier) ||
		i == QKeyEvent(QEvent::KeyRelease,
					   Game::configKey,
					   Qt::NoModifier)){
		return true;
	}
	Player* p;
	foreach(p, players){
		if(p->canHandleInput(i))
			return true;
	}
	return false;
}

void Game::setCurrentMode(Mode *current)
{
	transitionState = 0;
	next = current;
	emit modeHasChanged();
}

bool Game::updateGame()
{
	bool _return = false;
	if(!configDialog->isVisible()){
		if(next != NULL && transitionState<1){
			if(transitionState < 0.8){
				_return = current->updateEndTransition(transitionState);
			}
			else if(transitionState < 1){
				_return =  current->updateEndTransition(transitionState) ||
						next->updateStartTransition(transitionState-0.8);
			}
			else{
				_return = next->updateStartTransition(transitionState-0.8);
			}
			transitionState += transitionInc;
			if(transitionState>=1.6){
				current = next;
				next = NULL;
				emit modeHasChanged();
			}
		}
		else if(current != NULL){
			_return = current->update();
		}
	}
	return _return;
}

void Game::showConfigDialog()
{
	configDialog->clearPlayers();
	int i;
	configDialog->addPlayer(players[0],true,true);
	for(i=1;i<players.length();i++)
		configDialog->addPlayer(players[i],false,players[i]->isInGame());
	configDialog->setLevelFile(currentLevelFile);
	configDialog->show();
}

QStringList Game::shipNames()
{
	return shipModels.keys();
}

QStringList Game::weaponNames()
{
	return weaponModels.keys();
}

Ship *Game::shipModelFromName(QString name)
{
	return shipModels.value(name);
}

Weapon *Game::weaponModelFromName(QString name)
{
	return weaponModels.value(name);
}

const Renderer *Game::currentRenderer()
{
	if(next)
		return new DoubleRenderer(*current->renderer(),
									*next->renderer());
	return current->renderer();
}

bool Game::eventFilter(QObject *, QEvent * event)
{
	QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
	if(keyEvent){
		if(keyEvent->type() == QEvent::KeyRelease){
			if (keyEvent->key() == Game::configKey){
				showConfigDialog();
			}
		}
	}
	QInputEvent* e = dynamic_cast<QInputEvent*>(event);
	if(e != NULL){
		current->inputHandle(e);
		return true;
	}
	return false;
}

void Game::setConfig()
{
	configDialog->save();
	configDialog->close();
	if(!configDialog->levelFile().isEmpty())
		currentLevelFile = configDialog->levelFile();
	this->reload();
}

