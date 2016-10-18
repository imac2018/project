#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QGLWidget>
#include <QtOpenGL>
#include <QPointF>

class Renderer : public QGLWidget
{
	Q_OBJECT

	QPointF point;
	QTimer timer;
	float millisecondDelay;
public:
	explicit Renderer(QWidget *parent = 0, float framePerSecond = 60);
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintEvent(QPaintEvent *);

signals:

public slots:
	void animate();
};

#endif // RENDERER_H
