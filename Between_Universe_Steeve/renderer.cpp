#include "renderer.h"
#include <qpainter.h>

Renderer::Renderer(QWidget *parent, float framePerSecond) :
	QGLWidget(parent),
	point(QPointF(0,0)),
	millisecondDelay(1000.0/framePerSecond)
{

}

void Renderer::initializeGL()
{
	// Set up the rendering context, define display lists etc.:

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	timer.setSingleShot(false);
	QObject::connect(&timer, SIGNAL(timeout()),this,SLOT(animate()));
	timer.start(millisecondDelay);
}

void Renderer::resizeGL(int w, int h)
{
	// setup viewport, projection etc.:
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Renderer::paintEvent(QPaintEvent *)
{

	QPainter painter;
	painter.begin(this);
	painter.setBackground(QBrush(QColor(0,0,0)));
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(QRect(point.toPoint(),QSize(20,20)), QBrush(QColor("red")));
	painter.end();
}

void Renderer::animate(){
	point += QPointF(2,2);
	update();
}

