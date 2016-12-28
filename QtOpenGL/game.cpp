#include <assimp/types.h>

#include <QKeyEvent>
#include <QWidget>

#include "game.h"
#include "mode.h"
#include "gui.h"
#include "tools.h"

#define transitionInc 0.05f

void Game::transitionOutUpdate()
{
	float light = _renderer.globalIllumination.lightnessF();
	light -= transitionInc;
	if(light<=0){
		current->clear(*this);
		delete current;
		current = next;
		next = NULL;
		_renderer.resetGlobal();
		_renderer.lock = true;
		state = Loading;
		loadingState = 0;
	}
	else{
		_renderer.globalIllumination = QColor::fromRgbF(light,light,light);
		current->update(*this);
	}
}

void Game::transitionInUpdate()
{
	float light = _renderer.globalIllumination.lightnessF();
	light += transitionInc;
	if(light>=1){
		_renderer.lock = false;
		state = Normal;
	}
	else{
		_renderer.globalIllumination = QColor::fromRgbF(light,light,light);
		current->update(*this);
	}
}

void Game::loadingUpdate()
{
	current->initialize(*this, loadingState);

	loadingDonut->transformM.rotate(10,0,0,1);
	if(loadingState>=1){
		_renderer.globalIllumination = QColor::fromRgbF(0,0,0);
		state = TransitionIn;
	}
}

void Game::renderLoading()
{
	updateProjection();
	loadingRenderer.initRender();
	loadingRenderer.draw(loadingDonut);
	loadingRenderer.endRender();
}

QPointF Game::windowToGL(QPointF p)
{
	return QPointF(((p.x() - currentBound.x()) / currentBound.width())*2.-1.,
				   ((p.y() - currentBound.y()) / currentBound.height())*2.-1.);

}

Game::Game(QWidget *view, Mode* firstMode)
	: mainWindow(view), current(firstMode),
	  next(NULL), state(Loading), loadingState(0),
	  loadingDonut(NULL)
{
	rotationMatrix.rotate(2,0,0,1);
}

void Game::initializeRender()
{
	if(mainWindow){
		resizeHandler(mainWindow->width(), mainWindow->height());
	}
	else{
		resizeHandler(720, 480);
	}
	_renderer.initialize();
	loadingRenderer.initialize();
	loadingDonut = new Image(loadingRenderer, QRectF(-0.25,-0.25,0.5,0.5),
							  ":/assets/loading.png");
	loadingRenderer.addObject(*loadingDonut);
	loadingRenderer.initializeVBO();
	loadingRenderer.initializeVAO();
}

void Game::changeMode(Mode *m)
{
	next = m;
	state = TransitionOut;
}

void Game::updateGame()
{
	switch(state){
	case Normal:
		current->update(*this);
		break;
	case TransitionOut:
		transitionOutUpdate();
		break;
	case TransitionIn:
		transitionInUpdate();
		break;
	case Loading:
		loadingUpdate();
		break;
	}

}

void Game::render()
{
	switch(state){
	case Normal:
	case TransitionOut:
	case TransitionIn:
		current->render(*this);
		break;
	case Loading:
		renderLoading();
		break;
	}
}

void Game::resizeHandler(int windowWidth, int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	int height = (windowWidth*9)/16;
	int width = windowWidth;
	if(height>windowHeight){
		height = windowHeight;
		width = (windowHeight * 16) / 9;
	}
	currentBound = QRectF(windowWidth/2 - width/2,windowHeight/2 - height/2,width, height);
	glViewport((int)currentBound.x(),(int)currentBound.y(),
			   (int)currentBound.width(),(int)currentBound.height());
	updateProjection();
}

void Game::updateProjection()
{
	switch(_renderer.projectionType){
	case Renderer::Perspective:
		_renderer.setPerspective(70,16.f/9.f,1,500);
		break;
	case Renderer::Ortho:
		// 1.78 = 16/9
		_renderer.setOrtho(-1,1,1,-1,0,50);
		break;
	case Renderer::Frustrum:
		_renderer.setFrustum(-1,1,1,-1,0,50);
		break;
	default:
		//renderer.resetProjection();
		break;
	}
	loadingRenderer.projectionType = Renderer::Ortho;
	loadingRenderer.setOrtho(-1,1,1,-1,0,50);
}

Renderer &Game::renderer()
{
	return _renderer;
}

bool Game::eventFilter(QObject * source, QEvent * event)
{
	if(event->type() == QEvent::KeyRelease){
		if(dynamic_cast<QKeyEvent*>(event)->key() == Qt::Key_F && mainWindow){
			if(mainWindow->isVisible()){
				if(mainWindow->isFullScreen())
					mainWindow->showNormal();
				else
					mainWindow->showFullScreen();
				return true;
			}
		}
	}
	else if(event->type() == QEvent::Resize){
		if(source == mainWindow){
			QSize s = dynamic_cast<QResizeEvent*>(event)->size();
			//resizeHandler(s.width(),s.height());
			return false;
		}
	}
	else{
		QMouseEvent* mouseE = dynamic_cast<QMouseEvent*>(event);
		if(mouseE){
			QPointF mousePos = windowToGL(QPointF(mouseE->pos()));
			/*qDebug() << "-----";
			qDebug() << mouseE->x() << " " << mouseE->y();
			qDebug() << mousePos.x() << " " << mousePos.y();*/
			return current->inputHandle(*this, new QMouseEvent(mouseE->type(),mousePos,
														mouseE->button(),mouseE->buttons(),
														mouseE->modifiers()));
		}

		QInputEvent* e = dynamic_cast<QInputEvent*>(event);
		if(e){
			return current->inputHandle(*this, e);
		}
	}
	return false;
}
