#include "ship.h"
#include <memory>

const int superModeWaitDelay = 100;

Ship::Ship(float acceleration, float power, float life, int superModeDelay,
		   float accelerateAmmount, float powerUpAmmount,
		   Weapon *mainWeapon, Weapon *secondWeapon)
	: position(0,0), bound(position,QSizeF(70,30)), acceleration(acceleration), power(power),
	  life(life), superModeDelay(superModeDelay), accelerateAmmount(accelerateAmmount),
	  powerUpAmmount(powerUpAmmount), mainWeapon(mainWeapon), secondWeapon(secondWeapon),
	  sprite(":/images/wolf45J.png")
{
}

void Ship::paint(QPainter &context) const
{
	//context.drawRect(this->bound);
	context.drawImage(this->bound,sprite);
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

	this->position += velocity;
	this->bound.moveTo(position);

	if(mainWeapon->activated())
		mainWeapon->shoot(*world,position);
	if(secondWeapon->activated())
		secondWeapon->shoot(*world,position);
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
	std::unique_ptr<float> radius(new float);
	*radius = bound.width();
	return new ColliderRect(radius.get(),&position,&bound);
}

void Ship::moveForward()
{
	velocity.setX(acceleration);
}

void Ship::moveBackward()
{
	velocity.setX(-acceleration);
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

void Ship::handleCollision(SpaceObject *o)
{

}

void Ship::stopPowerUp(){
	power -= powerUpAmmount;
	powerUpCounter = -superModeWaitDelay;
}
