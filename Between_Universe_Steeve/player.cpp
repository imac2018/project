#include "player.h"
#include <QKeyEvent>
#include <QDebug>

#define TYPE_MASK 0x00ff0000
#define MODIFIER_MASK 0xfe000000
#define SPECIAL_MASK 0x0100ffff
// if an event doesn't validate this mask, it cannot be an user control

Player::Player() : ship(NULL)
{

}

void Player::setShip(Ship *ship)
{
	this->ship = ship;
}

void Player::addShipToGalata(Galata &level)
{
	level.addShip(ship);

}

void Player::addControl(Input control, ShipAction shipAction)
{
	controls.insert(control,shipAction);
}

bool Player::inputHandle(QInputEvent *event)
{
	QMap<Input,ShipAction>::const_iterator i = controls.find(event);
	if (i != controls.end()) {
		i.value()(*ship);
		return true;
	}
	else
		return false;
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
		code = 1;
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
	code += e->modifiers();

	return code;
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

long Input::hashcode() const
{
	return _hashcode;
}

