#include "enemy.h"
#include "tools.h"
#include "audio.h"

int Enemy::findPlayer(Level &l, int x, int y, int distance, QPoint prevPos)
{
	if(distance<=0) return -1;
	QPoint pos;
	for(int i = 0;i<OrientationCount;i++){
		pos = QPoint(x,y) + Map::getMoveFromOrientation((Orientation)i);
		if(pos!=prevPos){
			if(map->getCase(pos).isReachable()){
				if(l.player().mapPosition() == pos){
					return i;
				}
				if(findPlayer(l,pos.x(),pos.y(),distance-1, QPoint(x,y))>=0)
					return i;
			}
		}
	}
	return -1;
}

Enemy::Enemy(QList<Object3D*> models, Collider* collider, Map *map, float initialY,
			 int life, Orientation direction)
	: MapElement(models, collider), life(life),
	  t(0), cycle(60), _direction(direction), map(map), rotationValue(PI_f),
	  initialY(initialY), targetDirection(-1), rotationSpeed(0),
	  translateSpeed(0), rotationTarget(PI_f)
{

}

void Enemy::setCycle(int cycle)
{
	if(cycle>=25){
		this->cycle = cycle;
	}
}


void Enemy::update(Level &l)
{
	if(t==0){
		if(targetDirection>=0){
			AudioPlayer* player = AudioManager::player("salutsalut.mp3");
			if(player){
				player->setVolume(70);
				player->play();
			}
		}
		if(mapPos + Map::getMoveFromOrientation(_direction) == l.player().mapPosition()){
			l.player().receiveDamage();
		}
	}
	if(t%20==1){
		targetDirection = findPlayer(l,mapPos.x(),mapPos.y(),5, QPoint(-1,-1));
	}
	if(targetDirection>=0){
		if(targetDirection-_direction>0){
			rotateRight();
		}
		else if(targetDirection-_direction<0){
			rotateLeft();
		}
		else{
			if(t%25==2)
				if((l.player().mapPosition() - mapPos).manhattanLength()>1)
					moveForward();
		}
	}
	if(rotationSpeed>0.01f){
		QMatrix4x4 rotation;
		if(rotationValue + rotationSpeed > rotationTarget){
			rotation.rotate((rotationTarget-rotationValue)*toDegCoef,0,1,0);
			rotationValue = rotationTarget;
			rotationSpeed = 0;
		}
		rotationValue+=rotationSpeed;
		rotation.rotate(rotationSpeed*toDegCoef,0,1,0);
		transformRight(rotation);
	}
	else if(rotationSpeed<-0.01f){
		QMatrix4x4 rotation;
		if(rotationValue + rotationSpeed < rotationTarget){
			rotation.rotate((rotationTarget-rotationValue)*toDegCoef,0,1,0);
			rotationValue = rotationTarget;
			rotationSpeed = 0;
		}
		rotationValue+=rotationSpeed;
		rotation.rotate(rotationSpeed*toDegCoef,0,1,0);
		transformRight(rotation);
	}

	//Translate update
	Orientation direction = this->_direction;
	if(translateSpeed<-0.01f){
		direction = (Orientation)((this->_direction+2)%(OrientationCount));
	}
	if(!isZero(translateSpeed)){
		QVector3D frontVector = Camera::computeFrontVector(rotationValue);
		QMatrix4x4 translation;
		switch(direction){
		case NORTH:
			if(z() + translateSpeed * frontVector.z() < translateTarget.z()){
				translation.translate(0,0,(translateTarget.z() - z()));
				translateSpeed = 0;
			}
			break;
		case EAST:
			if(x() + translateSpeed * frontVector.x() > translateTarget.x()){
				translation.translate((translateTarget.x() - x()),0,0);
				translateSpeed = 0;
			}
			break;
		case SOUTH:
			if(z() + translateSpeed * frontVector.z() > translateTarget.z()){
				translation.translate(0,0,(translateTarget.z() - z()));
				translateSpeed = 0;
			}
			break;
		case WEST:
			if(x() + translateSpeed * frontVector.x() < translateTarget.x()){
				translation.translate((translateTarget.x() - x()),0,0);
				translateSpeed = 0;
			}
			break;
		}
		translation.translate(frontVector*translateSpeed);
		transform(translation);
	}
	t++;
	t=t%cycle;
}

void Enemy::actionOnCollision(Player &)
{}

void Enemy::actionOnClick(Player &)
{
	life--;
}

bool Enemy::isDead()
{
	return life<=0;
}

bool Enemy::blockPath() const
{
	return true;
}

void Enemy::moveForward()
{
	if(map->getCase(mapPos + Map::getMoveFromOrientation(_direction))
			.isReachable() && isZero(translateSpeed) && isZero(rotationSpeed)){
		mapPos += Map::getMoveFromOrientation(_direction);
		translateTarget = map->caseCenter(mapPos.x(), mapPos.y());
		translateSpeed = 1.f;
	}
}

void Enemy::moveBack()
{
	if(map->getCase(mapPos - Map::getMoveFromOrientation(_direction))
			.isReachable()  && isZero(translateSpeed)  && isZero(rotationSpeed)){
		mapPos -= Map::getMoveFromOrientation(_direction);
		translateTarget = map->caseCenter(mapPos.x(), mapPos.y());
		translateSpeed = -1.f;
	}
}

void Enemy::setPosition(QPoint mapPos, QVector3D realPos, Orientation dir)
{
	resetTransformation();

	this->mapPos = mapPos;
	rotationSpeed = 0;
	translateSpeed = 0;

	_direction = NORTH;
	QMatrix4x4 matrix;
	matrix.rotate(90 * (dir-_direction),0,1,0);
	rotationTarget = PI_f + HALFPI_f * (dir-_direction);

	translateTarget = QVector3D(realPos.x(),initialY, realPos.z());
	matrix.translate(translateTarget);
	transform(matrix);
}

void Enemy::rotateLeft()
{
	if(isZero(rotationSpeed) && isZero(translateSpeed)){
		rotationTarget += HALFPI_f;
		if(_direction==NORTH)
			_direction = WEST;
		else
			_direction = (Orientation)(_direction-1);
		rotationSpeed = 0.2f;
	}
}

void Enemy::rotateRight()
{
	if(isZero(rotationSpeed) && isZero(translateSpeed)){
		rotationTarget -= HALFPI_f;
		_direction = (Orientation)((_direction+1)%OrientationCount);
		rotationSpeed = -0.2f;
	}
}

Orientation Enemy::direction() const
{
	return _direction;
}
