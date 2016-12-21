#include "widget.h"
#include <QPainter>

Widget::Widget(const Renderer *renderer, QWidget *parent) :
	QWidget(parent), renderer(renderer)
{}

void Widget::paintEvent(QPaintEvent *)
{
	QPainter* p = new QPainter(this);
	renderer->paint(*p);
	delete p;
}

