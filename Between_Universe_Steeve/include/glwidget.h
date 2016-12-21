#ifndef RENDERER_H
#define RENDERER_H

#include <QGLWidget>
#include "renderer.h"

class GLWidget : public QGLWidget
{
public:
	const Renderer* renderer;
	explicit GLWidget(const Renderer* renderer, QWidget *parent = 0);
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintEvent(QPaintEvent *);
};

#endif // RENDERER_H
