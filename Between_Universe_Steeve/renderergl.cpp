#include "renderergl.h"
#include <qpainter.h>
#include <QDebug>

GLWidget::GLWidget(const Renderer &renderer, QWidget *parent) :
	QGLWidget(parent), renderer(renderer)
{}

void GLWidget::initializeGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void GLWidget::resizeGL(int w, int h)
{
	// setup viewport, projection etc.:
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWidget::paintEvent(QPaintEvent*)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	QPainter* p = new QPainter(this);
	renderer.paint(*p);
	delete p;
}

