#include "glwidget.h"
#include <qpainter.h>

GLWidget::GLWidget(const Renderer *renderer, QWidget *parent) :
	QGLWidget(parent), renderer(renderer)
{}

void GLWidget::initializeGL()
{
	QGLFormat format;
	format.setDoubleBuffer(true);
	format.setDepth(false);
	format.setAlpha(false);
	format.setSampleBuffers(true);
	format.setSamples(4);
	setFormat(format);
}

void GLWidget::resizeGL(int w, int h)
{
	// setup viewport, projection etc.:
	glViewport(0, 0, (GLint)w, (GLint)h);
}

void GLWidget::paintEvent(QPaintEvent*)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	QPainter* p = new QPainter(this);
	renderer->paint(*p);
	delete p;
}

