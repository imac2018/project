#include "ship.h"
#include <memory>
#include "player.h"

const int superModeWaitDelay = 100;

Ship::Ship(Player *parent, float acceleration, float power, float life, int superModeDelay,
		   float accelerateAmmount, float powerUpAmmount,
		   Weapon *mainWeapon, Weapon *secondWeapon)
	: parent(parent), size(70,45), acceleration(acceleration), power(power),
	  life(life), superModeDelay(superModeDelay), accelerateAmmount(accelerateAmmount),
	  powerUpAmmount(powerUpAmmount), mainWeapon(mainWeapon), secondWeapon(secondWeapon),
	  sprite(":/images/wolf45J.png"), deadState(0)
{
}

Ship::Ship(float acceleration, float power, float life, int superModeDelay,
		   float accelerateAmmount, float powerUpAmmount, Weapon *mainWeapon, Weapon *secondWeapon)
	: Ship(NULL,acceleration, power, life, superModeDelay, accelerateAmmount, powerUpAmmount,
		   mainWeapon, secondWeapon)
{

}

void Ship::paint(QPainter &context) const
{
	//context.drawRect(this->bound);
	context.drawImage(QRectF(QPointF(0,0), size),sprite);
}

void Ship::animate()
{
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
		mainWeapon->shoot(this,*world,actualPos);
	if(secondWeapon->activated())
		secondWeapon->shoot(this,*world,actualPos);
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
	return new ColliderRect(QRectF(actualPosition(), size));
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
	secondWeapon->activate();
}

void Ship::mainShootStop()
{
	mainWeapon->stop();
}

void Ship::secondShootStop()
{
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
		power += powerUpAmmount;
		powerUpCounter--;
	}
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

void Ship::setParentPlayer(Player *parent)
{
	this->parent = parent;
}

void Ship::addPoints(int points)
{
	parent->addPoints(points);
}

void Ship::handleCollision(SpaceObject *)
{}

void Ship::stopPowerUp(){
	power -= powerUpAmmount;
	powerUpCounter = -superModeWaitDelay;
}

Ship::Ship()
{

}
