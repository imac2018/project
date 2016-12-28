#include "level.h"
#include "game.h"
#include "object3d.h"
#include "tools.h"
#include <QMouseEvent>

Level::Level()
	: direction(NORTH), initialY(2.f)
{

}

void Level::initialize(Game & game, float &currentState)
{
	Renderer& renderer = game.renderer();

	if(currentState<0.1f){
		renderer.resetGlobal();
		renderer.projectionType = Renderer::Ortho;
		game.updateProjection();
		currentState = 0.25f;
	}
	else if(currentState<0.4f){
		playGui.append(new Button(renderer,"ok",QRectF(-0.25f,-0.75f,0.5f,0.1f)));
		renderer.viewCam.reset();
		rotationTarget = renderer.viewCam.rotationLeft();
		rotationSpeed = 0;
		translateSpeed = 0;
		translateTarget = renderer.viewCam.position();
		upSpeed = 0;
		currentState = 0.4f;
	}
	else if(currentState<0.5f){
		foreach(Object3D* o, playGui.objects()){
			renderer.addObject(*o);
		}
		currentState = 0.5f;
	}
	else if(currentState<0.6){
		currentState+=0.01f;
	}
	else if(currentState<0.75f){
		Object3D* o = new Object3D(renderer.makeTexturedFace(90, 0,0,500.f));
		o->texture= renderer.addTexture(":/assets/sol.jpg");
		o->transformM.translate(0,-1.f,0);
		objects.append(o);
		currentState = 0.8f;
	}
	else if(currentState<0.95f){
		currentState = 0.96f;
	}
	else{
		renderer.initializeVBO();
		renderer.initializeVAO();
		renderer.resetGlobal();
		renderer.viewCam.translateUp(initialY);
		currentState = 1;
	}
}

bool Level::inputHandle(Game & game, QInputEvent * e)
{
	Renderer& renderer = game.renderer();
	QMouseEvent* mouseE = dynamic_cast<QMouseEvent*>(e);
	if(mouseE){
		if(mouseE->type() == QEvent::MouseMove){
			return playGui.handleMouseMove(mouseE);
		}
		else if(mouseE->type() == QEvent::MouseButtonPress){
			return playGui.handleMousePress(mouseE);
		}
		else if(mouseE->type() == QEvent::MouseButtonRelease){
			return playGui.handleMouseRelease(mouseE);
		}
	}
	Camera& cam = renderer.viewCam;
	QKeyEvent* keyE = dynamic_cast<QKeyEvent*>(e);
	if(keyE && isZero(rotationSpeed) && isZero(rotationSpeed)){
		if(keyE->type() == QEvent::KeyPress){
			switch (keyE->key()){
			case Qt::Key_Left:
				rotationTarget = cam.rotationLeft() + HALFPI_f;
				direction = (Orientation)abs(((direction-1)%OrientationCount));
				rotationSpeed = +0.2f;
				return true;
			case Qt::Key_Right:
				rotationTarget = cam.rotationLeft() - HALFPI_f;
				direction = (Orientation)abs(((direction+1)%OrientationCount));
				rotationSpeed = -0.2f;
				return true;
			case Qt::Key_Up:
				translateTarget = cam.position() + (cam.frontVector() * 10.f);
				translateSpeed = 1.f;
				upSpeed = 0.2f;
				return true;
			case Qt::Key_Down:
				translateTarget = cam.position() + (cam.frontVector() * -10.f);
				translateSpeed = -1.f;
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

bool Level::testTargetPos(const Camera& c) const{
	return isZero(c.x() - translateTarget.x()) &&
			isZero(c.y() - translateTarget.y()) &&
			isZero(c.z() - translateTarget.z());

}

void Level::update(Game & game)
{
	Renderer& renderer = game.renderer();
	Camera& cam = renderer.viewCam;
	if(rotationSpeed>0.01f){
		if(cam.rotationLeft() + rotationSpeed > rotationTarget){
			cam.rotateLeft(rotationTarget - cam.rotationLeft());
			rotationSpeed = 0;
		}
		cam.rotateLeft(rotationSpeed);
	}
	else if(rotationSpeed<-0.01f){
		if(cam.rotationLeft() + rotationSpeed < rotationTarget){
			cam.rotateLeft(rotationTarget - cam.rotationLeft());
			rotationSpeed = 0;
		}
		cam.rotateLeft(rotationSpeed);
	}
	Orientation direction = this->direction;
	if(translateSpeed<-0.01f){
		direction = (Orientation)((this->direction+2)%(OrientationCount));
	}
	if(!isZero(translateSpeed)){
		switch(direction){
		case NORTH:
			if(cam.z() + (translateSpeed * cam.frontVector()).z() < translateTarget.z()){
				cam.moveFront((translateTarget.z() - cam.z())/cam.frontVector().z());
				translateSpeed = 0;
			}
			break;
		case EAST:
			if(cam.x() + (translateSpeed * cam.frontVector()).x() > translateTarget.x()){
				cam.moveFront((translateTarget.x() - cam.x())/cam.frontVector().x());
				translateSpeed = 0;
			}
			break;
		case SOUTH:
			if(cam.z() + (translateSpeed * cam.frontVector()).z() > translateTarget.z()){
				cam.moveFront((translateTarget.z() - cam.z())/cam.frontVector().z());
				translateSpeed = 0;
			}
			break;
		case WEST:
			if(cam.x() + (translateSpeed * cam.frontVector()).x() < translateTarget.x()){
				cam.moveFront((translateTarget.x() - cam.x())/cam.frontVector().x());
				translateSpeed = 0;
			}
			break;
		}
		cam.moveFront(translateSpeed);

		if(upSpeed>0.01f){
			cam.translateUp(upSpeed);
			if(cam.y()>=initialY+0.75f){
				cam.translateUp(initialY+0.75f - cam.y());
				upSpeed *= -1;
			}
		}
		else if(upSpeed<-0.01f){
			cam.translateUp(upSpeed);
			if(cam.y()<=initialY){
				cam.translateUp(initialY - cam.y());
				upSpeed *= -1;
			}
		}
	}
	else
	{
		if(!isZero(cam.y()-initialY))
			cam.translateUp(initialY-cam.y());
	}

}

void Level::render(Game & game)
{
	Renderer& renderer = game.renderer();
	renderer.initRender();

	glEnable(GL_DEPTH_TEST);
	renderer.projectionType = Renderer::Perspective;
	game.updateProjection();
	renderer.bindCamera();
	renderer.draw(objects);

	glDisable(GL_DEPTH_TEST);
	renderer.projectionType = Renderer::Ortho;
	game.updateProjection();
	renderer.releaseCamera();
	renderer.draw(playGui.objects());

	renderer.endRender();
}

void Level::clear(Game & game)
{
	Renderer& renderer = game.renderer();
	renderer.clearVAO();
	renderer.clearVBO();
	renderer.clearTextures();
}
