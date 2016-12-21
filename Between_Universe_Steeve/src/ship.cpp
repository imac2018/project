#include "ship.h"
#include <memory>
#include "player.h"

const int superModeWaitDelay = 100;

Ship::Ship(Player *parent, const QString& name, const QImage& sprite,
		   float acceleration, float power, int life, int superModeDelay,
		   float accelerateAmmount, float powerUpAmmount,
		   Weapon *mainWeapon, Weapon *secondWeapon)
	: SpaceObject(sprite.size()), parent(parent), _name(name),
	  acceleration(acceleration),  _power(power), life(life),
	  superModeDelay(superModeDelay),
	  accelerateAmmount(accelerateAmmount), powerUpAmmount(powerUpAmmount),
	  sprite(sprite), deadState(0), defense(0)
{
	colliderBorder.append(Space::origin);
	colliderBorder.append(QPoint(_size.width(), _size.height()*0.5));
	colliderBorder.append(QPoint(_size.width(), _size.height()));
	colliderBorder.append(QPoint(0,_size.height()));
	colliderBorder.append(Space::origin);
	if(mainWeapon)
		this->setMainWeapon(*mainWeapon);
	else
		this->mainWeapon = NULL;
	if(secondWeapon)
		this->setSecondWeapon(*secondWeapon);
	else
		this->secondWeapon = NULL;
}

Ship::Ship(const QString &name, const QImage &sprite, float acceleration,
		   float power, int life, int superModeDelay,
		   float accelerateAmmount, float powerUpAmmount, Weapon *mainWeapon,
		   Weapon *secondWeapon)
	: Ship(NULL,name, sprite, acceleration, power, life, superModeDelay,
		   accelerateAmmount, powerUpAmmount,
		   mainWeapon, secondWeapon)
{

}

void Ship::paint(QPainter &context) const
{


	if(!isDead()){
		context.drawImage(QRectF(Space::origin, _size),sprite);
		foreach (Bonus* b, bonus) {
			b->paint(context);
		}
	}
	else {
		context.setOpacity(1-deadState);
		context.drawImage(QRectF(Space::origin, _size),sprite);
		context.translate(_size.width()*0.5, _size.height()*0.5);
		drawExplosion(context,_size.height(),deadState);
	}
}

void Ship::animate()
{
	if(!isDead()){
		if(accelerateCounter<0){
			accelerateCounter++;
			if(accelerateCounter==0){
				accelerateCounter =	superModeDelay;
			}
		}
		else if(accelerateCounter<superModeDelay){
			accelerateCounter--;
			if(accelerateCounter==0){
				stopAccelerate();
			}
		}
		if(powerUpCounter<0){
			powerUpCounter++;
			if(powerUpCounter==0){
				powerUpCounter =	superModeDelay;
			}
		}else if(powerUpCounter<superModeDelay){
			powerUpCounter--;
			if(powerUpCounter==0){
				stopPowerUp();
			}
		}

		QPointF actualPos = actualPosition();

		this->moveTo(actualPos + velocity);

		if(mainWeapon->activated())
			mainWeapon->shoot(*this,*world,actualPos);
		if(secondWeapon)
			if(secondWeapon->activated())
				secondWeapon->shoot(*this,*world,actualPos);

		foreach (Bonus* b, bonus) {
			b->animate();
		}

		QMutableListIterator<Bonus*> i(bonus);
		while(i.hasNext()){
			Bonus* b = i.next();
			if(b->isOver()){
				b->unmodifyShip();
				i.remove();
			}
		}
	}
	else{
		deadState += 0.06f;
	}
}

void Ship::handleDeath()
{
	if(parent){
		parent->handleShipDeath();
	}
}

void ShipActions::moveForward(Ship& ship)
{
	ship.moveForward();
}

void ShipActions::moveBackward(Ship& ship)
{
	ship.moveBackward();
}

void ShipActions::moveUp(Ship& ship)
{
	ship.moveUp();
}

void ShipActions::moveDown(Ship& ship)
{
	ship.moveDown();
}

void ShipActions::stopForward(Ship& ship)
{
	ship.stopForward();
}

void ShipActions::stopBackward(Ship& ship)
{
	ship.stopBackward();
}

void ShipActions::stopUp(Ship& ship)
{
	ship.stopUp();
}

void ShipActions::stopDown(Ship& ship)
{
	ship.stopDown();
}

void ShipActions::mainWeaponActivate(Ship& ship)
{
	ship.mainShootActivate();
}

void ShipActions::secondWeaponActivate(Ship& ship)
{
	ship.secondShootActivate();
}

void ShipActions::mainWeaponStop(Ship& ship)
{
	ship.mainShootStop();
}

void ShipActions::secondWeaponStop(Ship& ship)
{
	ship.secondShootStop();
}


void ShipActions::accelerate(Ship& ship){
	ship.accelerate();
}

void ShipActions::powerUp(Ship& ship){
	ship.powerUp();
}

ColliderObject *Ship::collider() const
{
	//return new ColliderRect(QRectF(actualPosition(), size));

	return new ColliderPolygon(_size.width() * 0.5,
							   QPointF(_size.width()*0.5, _size.height()*0.5),
							   colliderBorder);
}

void Ship::moveForward()
{
	velocity.setX(acceleration);
}

void Ship::moveBackward()
{
	velocity.setX(-acceleration*0.75);
}

void Ship::moveUp()
{
	velocity.setY(-acceleration);
}

void Ship::moveDown()
{
	velocity.setY(acceleration);
}

void Ship::stopForward()
{
	velocity.setX(0);
}

void Ship::stopBackward()
{
	velocity.setX(0);
}

void Ship::stopUp()
{
	velocity.setY(0);
}

void Ship::stopDown()
{
	velocity.setY(0);
}

void Ship::mainShootActivate()
{
	mainWeapon->activate();
}

void Ship::secondShootActivate()
{
	if(secondWeapon)
		secondWeapon->activate();
}

void Ship::mainShootStop()
{
	mainWeapon->stop();
}

void Ship::secondShootStop()
{
	if(secondWeapon)
		secondWeapon->stop();
}



void Ship::accelerate(){
	if(accelerateCounter==superModeDelay){
		if(abs(velocity.x())>0)
			velocity.setX(velocity.x() * accelerateAmmount);
		if(abs(velocity.y())>0)
			velocity.setY(velocity.y() * accelerateAmmount);
		acceleration *= accelerateAmmount;
		accelerateCounter--;
	}
}

void Ship::stopAccelerate(){
	if(abs(velocity.x())>0)
		velocity.setX(velocity.x() / accelerateAmmount);
	if(abs(velocity.y())>0)
		velocity.setY(velocity.y() / accelerateAmmount);
	acceleration /= accelerateAmmount;
	accelerateCounter = -superModeWaitDelay;

}

void Ship::powerUp(){
	if(powerUpCounter==superModeDelay){
		_power += powerUpAmmount;
		powerUpCounter--;
	}
}

void Ship::levelUp()
{
	mainWeapon->levelUp();
	if(secondWeapon)
		secondWeapon->levelUp();
}
void Ship::addBonus(Bonus *b)
{
	this->bonus.append(b);
	b->modifyShip();
}

void Ship::setMainWeapon(const Weapon& w)
{
	this->mainWeapon = w.instanciate(QPointF(this->_size.width() * 0.5,
											  this->_size.height() * 0.9));
}

void Ship::setSecondWeapon(const Weapon &w)
{
	this->secondWeapon = w.instanciate(QPointF(this->_size.width() * 0.4,
											  this->_size.height() * 0.95));
}

bool Ship::isDead() const
{
	return life <= 0;
}

bool Ship::isDefinitelyDead() const
{
	return life <= 0 && deadState >= 1;
}

bool Ship::isInView(float, float) const
{
	return true;
}

float Ship::power() const
{
	return _power;
}

QString Ship::name() const
{
	return _name;
}

void Ship::setParentPlayer(Player *parent)
{
	this->parent = parent;
}

void Ship::addPoints(int points)
{
	parent->addPoints(points);
}

void Ship::assert()
{
	if(!mainWeapon)
		throw std::exception(QString("The ship of " + parent->name() +
							 " has no weapon").toStdString().c_str());
	if(!parent)
		throw std::exception(QString("The ship of " + _name +
							 " has no player").toStdString().c_str());
}

Ship *Ship::instanciate(Player* parent) const
{
	Ship* newShip = new Ship(parent,_name, sprite, acceleration,_power,life,
							 superModeDelay, accelerateAmmount,powerUpAmmount,
							 mainWeapon,secondWeapon);
	return newShip;
}

void Ship::damageShip(Enemy &, int damage)
{
	damage -= defense;
	if(damage > 0)
	{
		life -= damage;
	}
}

bool Ship::handleBulletCollision(SpaceObject *o, float damage)
{
	Enemy* e = dynamic_cast<Enemy*>(o);
	if(e){
		e->damageEnemy(*this, damage*_power);
		return true;
	}
	return false;
}

void Ship::handleCollision(SpaceObject *o)
{
	if(defense>0){
		Enemy* e = dynamic_cast<Enemy*>(o);
		if(e){
			e->damageEnemy(*this,defense);
			QList<Bonus*>::iterator i = bonus.begin();
			while(i!=bonus.end()){
				(*i)->handleCollision();
				i++;
			}
		}
	}
}

void Ship::stopPowerUp(){
	_power -= powerUpAmmount;
	powerUpCounter = -superModeWaitDelay;
}

void Bonus::addShipDefense(Ship& ship) const
{
	ship.defense += 1000;
}

void Bonus::removeShipDefense(Ship &ship) const
{
	ship.defense -= 1000;
}
