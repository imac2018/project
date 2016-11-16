#ifndef RENDER_H
#define RENDER_H

class QPainter;

class Renderer
{
public:
	virtual void paint(QPainter& context) const = 0;
	Renderer(){}
};

#endif // RENDER_H
