#include "player.h"
#include "tools.h"
#include "audio.h"

Player::Player(Level &parent, Map *map)
	: _direction(NORTH), initialY(5.f), map(map),
      caseTarget(NULL), _duffCount(0), parent(parent),
	  _life(5)
{}

void Player::setMap(Map *map)
{
	this->map = map;
}

bool Player::inputHandle(Game &, QInputEvent *e)
{
	QKeyEvent* keyE = dynamic_cast<QKeyEvent*>(e);
	if(keyE && isZero(rotationSpeed) && isZero(translateSpeed)){
		if(keyE->type() == QEvent::KeyPress){
			switch (keyE->key()){
			case Qt::Key_Left:
				rotateLeft();
				return true;
			case Qt::Key_Right:
				rotateRight();
				return true;
			case Qt::Key_Up:
				map->moveForward(*this);
				upSpeed = 0.2f;
				return true;
			case Qt::Key_Down:
				map->moveBack(*this);
				upSpeed = 0.2f;
				return true;
			default:
				break;
			}
		}
		else if(keyE->type() == QEvent::KeyRelease){

		}
	}
	return false;
}

void Player::update(Game &)
{
	// Rotation update
	if(rotationSpeed>0.01f){
		if(view.rotationLeft() + rotationSpeed > rotationTarget){
			view.rotateLeft(rotationTarget - view.rotationLeft());
			rotationSpeed = 0;
		}
		view.rotateLeft(rotationSpeed);
	}
	else if(rotationSpeed<-0.01f){
		if(view.rotationLeft() + rotationSpeed < rotationTarget){
			view.rotateLeft(rotationTarget - view.rotationLeft());
			rotationSpeed = 0;
		}
		view.rotateLeft(rotationSpeed);
	}

	//Translate update
	Orientation direction = this->_direction;
	if(translateSpeed<-0.01f){
		direction = (Orientation)((this->_direction+2)%(OrientationCount));
	}
	if(!isZero(translateSpeed)){
		switch(direction){
		case NORTH:
			if(view.z() + translateSpeed * view.frontVector().z() < translateTarget.z()){
				view.moveFront((translateTarget.z() - view.z())/view.frontVector().z());
				translateSpeed = 0;
			}
			break;
		case EAST:
			if(view.x() + translateSpeed * view.frontVector().x() > translateTarget.x()){
				view.moveFront((translateTarget.x() - view.x())/view.frontVector().x());
				translateSpeed = 0;
			}
			break;
		case SOUTH:
			if(view.z() + translateSpeed * view.frontVector().z() > translateTarget.z()){
				view.moveFront((translateTarget.z() - view.z())/view.frontVector().z());
				translateSpeed = 0;
			}
			break;
		case WEST:
			if(view.x() + translateSpeed * view.frontVector().x() < translateTarget.x()){
				view.moveFront((translateTarget.x() - view.x())/view.frontVector().x());
				translateSpeed = 0;
			}
			break;
		}
		view.moveFront(translateSpeed);

		if(upSpeed>0.01f){
			view.translateUp(upSpeed);
			if(view.y()>=initialY+0.75f){
				view.translateUp(initialY+0.75f - view.y());
				upSpeed *= -1;
			}
		}
		else if(upSpeed<-0.01f){
			view.translateUp(upSpeed);
			if(view.y()<=initialY){
				view.translateUp(initialY - view.y());
				upSpeed *= -1;
			}
		}
	}
	else
	{
		if(caseTarget){
			caseTarget->action(*this,parent);
			caseTarget = NULL;
		}
		if(!isZero(view.y()-initialY))
			view.translateUp(initialY-view.y());
	}
}

void Player::moveForward(Case* target)
{
	mapPos += Map::getMoveFromOrientation(_direction);
	caseTarget = target;
	translateTarget = map->caseCenter(mapPos.x(), mapPos.y());
	translateSpeed = 1.f;
}

void Player::moveBack(Case* target)
{
	mapPos -= Map::getMoveFromOrientation(_direction);
	caseTarget = target;
	translateTarget = map->caseCenter(mapPos.x(), mapPos.y());
	translateSpeed = -1.f;
}

void Player::setPosition(QPoint mapPos, QVector3D realPos, Orientation dir)
{
	view.reset();

	this->mapPos = mapPos;
	rotationSpeed = 0;
	translateSpeed = 0;
	upSpeed = 0;

    view.rotateLeft(-HALFPI_f * (dir-NORTH));
    _direction = dir;
	rotationTarget = view.rotationLeft();

	translateTarget = QVector3D(realPos.x(),initialY, realPos.z());
	view.setPosition(translateTarget);

}

void Player::rotateLeft()
{
	rotationTarget += HALFPI_f;
	if(_direction==NORTH)
		_direction = WEST;
	else
		_direction = (Orientation)(_direction-1);
	rotationSpeed = +0.2f;
}

void Player::rotateRight()
{
	rotationTarget -= HALFPI_f;
	_direction = (Orientation)((_direction+1)%OrientationCount);
	rotationSpeed = -0.2f;
}

void Player::incrementDuff()
{
	_duffCount++;
}

QPoint Player::mapPosition() const
{
	return mapPos;
}

QVector3D Player::realPosition() const
{
	return view.position();
}

int Player::duffCount() const
{
	return _duffCount;
}
int Player::life() const
{
	return _life;
}


const Camera &Player::camera() const
{
	return view;
}

void Player::receiveDamage()
{
	AudioPlayer* player = AudioManager::player("cri.mp3");
	if(player){
		player->setVolume(30);
		player->play();
	}
	_life--;
}

void Player::clear()
{
	_life = 5;
	_duffCount = 0;
}

Orientation Player::direction() const
{
	return _direction;
}
