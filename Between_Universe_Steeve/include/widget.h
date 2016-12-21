#ifndef RENDERERWIDGET_H
#define RENDERERWIDGET_H
#include <QWidget>
#include "renderer.h"

class Widget : public QWidget
{
	const Renderer* renderer;
public:
	Widget(const Renderer *renderer, QWidget* parent = NULL);

protected:
	void paintEvent(QPaintEvent*);
};

#endif // RENDERERWIDGET_H
