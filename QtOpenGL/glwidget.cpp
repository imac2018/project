#include "glwidget.h"
#include "glexception.h"
#include "menu.h"
#include <QApplication>
#include <QTimer>

GLWidget::GLWidget(const QGLFormat &format, QWidget *parent)
	: QGLWidget(format,parent), game(parent!=NULL?parent:this, new Menu())
{
	QTimer* timer = new QTimer(this);
	timer->start(1000/50);

	QObject::connect(timer,SIGNAL(timeout()),this,SLOT(update()));
	//installEventFilter(&game);
	QApplication::instance()->installEventFilter(&game);
}

void GLWidget::initializeGL()
{
	glClearColor(0.f,0.f,0.f,1.f);
	game.initializeRender();
	game.resizeHandler(parentWidget()->width(),parentWidget()->height());
}

void GLWidget::resizeGL(int w, int h)
{
	game.resizeHandler(w,h);
}

void GLWidget::paintGL()
{
	game.updateGame();
	game.render();
}

