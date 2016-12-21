#include "player.h"
#include <QKeyEvent>
#include <QSettings>

#define TYPE_MASK 0x00ff0000
#define MODIFIER_MASK 0xfe000000
#define SPECIAL_MASK 0x0100ffff
// if an event doesn't validate this mask, it cannot be an user control

Player::Player() : Player("",QColor(255,255,255),5)
{}

Player::Player(QString name, QColor color, int life, bool inGame)
	: _points(0), _name(name), _color(color), _life(life), shipModel(NULL),
	  mainWeapon(NULL), secondWeapon(NULL), currentShip(NULL), inGame(inGame)
{}

void Player::setShip(const Ship &shipModel)
{
	this->shipModel = &shipModel;

}

void Player::setMainWeapon(const Weapon &w)
{
	mainWeapon = &w;
	if(this->currentShip)
		this->currentShip->setMainWeapon(w);
}

void Player::setSecondWeapon(const Weapon &w)
{
	secondWeapon = &w;
	if(this->currentShip)
		this->currentShip->setSecondWeapon(w);
}

void Player::setLevel(Galata *level)
{
	this->level = level;
}

void Player::addShipToGalata()
{
	if(shipModel){
		currentShip = shipModel->instanciate(this);
		if(mainWeapon)
			currentShip->setMainWeapon(*mainWeapon);
		if(secondWeapon)
			currentShip->setSecondWeapon(*secondWeapon);
		level->addShip(currentShip);
	}
	else
		throw std::exception(QString("NullPointerException: Player "+_name+
							 " has no ship model").toStdString().c_str());
}

void Player::handleShipDeath()
{
	if(currentShip->isDefinitelyDead()){
		_life--;
		if(_life>0){
			addShipToGalata();
		}
		else{
			level->handlePlayerDeath(this);
		}
	}
}

void Player::addControl(const Input &control, ShipAction shipAction)
{
	//removeAction(shipAction);
	controls.insert(control,shipAction);
}

void Player::removeAction(ShipAction action)
{
	QMap<Input, ShipAction>::iterator i = controls.begin();
	while(i != controls.end()){
		void (*const* ptr1)(Ship&) = action.target<void(*)(Ship&)>();
		void (*const* ptr2)(Ship&) = i.value().target<void(*)(Ship&)>();
		if(ptr1 && ptr1 == ptr2){
			controls.erase(i);
			return;
		}
		i++;
	}
}

bool Player::inputHandle(QInputEvent *event)
{
	Input key(event);
	if(key.hashcode()){
		QMap<Input,ShipAction>::const_iterator i = controls.find(event);
		if (i != controls.end()) {
			i.value()(*currentShip);
			return true;
		}
	}
	return false;
}

bool Player::canHandleInput(const Input &i) const
{
	return controls.contains(i);
}

void Player::addPoints(unsigned int points)
{
	this->_points += points;
}

void Player::setName(const QString &name)
{
	_name = name;
}

void Player::setColor(const QColor &color)
{
	_color = color;
}

void Player::setLife(int life)
{
	_life = life;
}

void Player::putInGame(bool inGame)
{
	this->inGame = inGame;
}

unsigned int Player::points() const
{
	return _points;
}

QString Player::name() const
{
	return _name;
}

QColor Player::color() const
{
	return _color;
}

int Player::life() const
{
	return _life;
}

bool Player::isInGame() const
{
	return inGame;
}

/**
 * @brief Input::makeHashcode
 * make a hash code from Event which allow the comparison between two user input
 * @param e, event which we make the hashcode from
 * @return the corresponding hascode if the event can
 */
long Input::makeHashcode(const QInputEvent *e)
{
	long code;
	switch(e->type()){
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
		if(dynamic_cast<const QKeyEvent*>(e)->isAutoRepeat())
			code = 0;
		else
			code = dynamic_cast<const QKeyEvent*>(e)->key();
		break;
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
		code = dynamic_cast<const QMouseEvent*>(e)->button();
		break;
	case QEvent::TouchEnd:
	case QEvent::TouchBegin:
		code = dynamic_cast<const QTouchEvent*>(e)->touchPointStates();
		break;
	case QEvent::Wheel:
		if(dynamic_cast<const QWheelEvent*>(e)->delta()>0)
			code = 1;
		else
			code = 2;
		break;
	default:
		code = 0;
		break;
	}

	long type = ((long)e->type()) << 16;
	if (!(code & SPECIAL_MASK) ||
		!(type & TYPE_MASK))
		return 0;
	code += type;
	//code += e->modifiers();

	return code;
}

Input::Input() : event(QEvent::None), _hashcode(0)
{

}

Input::Input(const QInputEvent *e) : event(*e), _hashcode(makeHashcode(e))
{
}
Input::Input(const QInputEvent &e) : event(e), _hashcode(makeHashcode(&e))
{
}

bool Input::operator==(const Input &i2) const
{
	return this->_hashcode == i2._hashcode;
}

bool Input::operator!=(const Input &i2) const
{
	return this->_hashcode != i2._hashcode;
}

bool Input::operator<(const Input &i2) const
{
	return this->_hashcode < i2._hashcode;
}

Input &Input::operator=(Input i2)
{
	this->event = i2.event;
	this->_hashcode = i2._hashcode;
	return *this;
}

long Input::hashcode() const
{
	return _hashcode;
}

void Input::save(QSettings &settings, const QString &prefix)
{
	settings.beginGroup(prefix);
	long code;
	switch(event.type()){
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
		if(dynamic_cast<const QKeyEvent*>(&event)->isAutoRepeat())
			code = 0;
		else
			code = dynamic_cast<const QKeyEvent*>(&event)->key();
		break;
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
		code = dynamic_cast<const QMouseEvent*>(&event)->button();
		break;
	case QEvent::TouchEnd:
	case QEvent::TouchBegin:
		code = dynamic_cast<const QTouchEvent*>(&event)->touchPointStates();
		break;
	case QEvent::Wheel:
		if(dynamic_cast<const QWheelEvent*>(&event)->delta()>0)
			code = 1;
		else
			code = 2;
		break;
	default:
		code = 0;
		break;
	}
	long type = ((long)event.type()) << 16;
	if (!(code & SPECIAL_MASK) ||
		!(type & TYPE_MASK))
		return;
	settings.setValue("code",code);
	settings.setValue("type",event.type());
	settings.setValue("modifiers",(int)event.modifiers());
	settings.endGroup();
}

Input Input::load(const QSettings &settings, const QString &prefix)
{
	QEvent::Type type = (QEvent::Type)settings.value(prefix+"/type").toInt();
	QInputEvent* e;
	switch(type){
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
		e = new QKeyEvent(type,settings.value(prefix+"/code").toInt(),
								(Qt::KeyboardModifiers)
									settings.value(prefix+"/modifiers").toInt());
		break;
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
		e = new QMouseEvent(type,QPointF(),
							(Qt::MouseButton)
								settings.value(prefix+"/code").toInt(),
							Qt::NoButton,
							(Qt::KeyboardModifiers)
								settings.value(prefix+"/modifiers").toInt());
		break;
	case QEvent::TouchEnd:
	case QEvent::TouchBegin:
		e = new QTouchEvent(type,0,
							(Qt::KeyboardModifiers)
								settings.value(prefix+"/modifiers").toInt(),
							(Qt::TouchPointStates)
							settings.value(prefix+"/code").toInt());
		break;
	case QEvent::Wheel:
		e = new QWheelEvent(QPointF(),
							settings.value(prefix+"/code").toInt()==1?1:-1,
							Qt::NoButton,
							(Qt::KeyboardModifiers)
								settings.value(prefix+"/modifiers").toInt());
		break;
	default:
		e = new QInputEvent(QEvent::None);
		break;
	}
	Input _retour(e);
	delete e;
	return _retour;
}

