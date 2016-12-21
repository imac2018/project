#ifndef RENDER_H
#define RENDER_H

class QPainter;

/**
 * @brief The Renderer is an interface which ask a paint function
 */
class Renderer
{
public:
	virtual void paint(QPainter& context) const = 0;
	Renderer(){}
};

class DoubleRenderer : public Renderer
{
	const Renderer& render1;
	const Renderer& render2;
public:
	DoubleRenderer(const Renderer& render1, const Renderer& render2);
	virtual void paint(QPainter& context) const;

};

#endif // RENDER_H
