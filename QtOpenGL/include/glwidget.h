#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "game.h"

class GLWidget : public QGLWidget
{

	Game game;

public:
	GLWidget(const QGLFormat &format, QWidget* parent=Q_NULLPTR);

protected:
	virtual void initializeGL();
	virtual void resizeGL( int w, int h );
	virtual void paintGL();
};

#endif // GLWIDGET_H
