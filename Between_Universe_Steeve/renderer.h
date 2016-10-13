#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QGLWidget>
#include <QtOpenGL>

class Renderer : public QGLWidget
{
	Q_OBJECT
public:
	explicit Renderer(QWidget *parent = 0);
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintEvent(QPaintEvent * event);

signals:

public slots:

};

#endif // RENDERER_H
