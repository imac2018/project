#include "renderer.h"

DoubleRenderer::DoubleRenderer(const Renderer &render1,
							   const Renderer &render2)
	: render1(render1), render2(render2)
{

}

void DoubleRenderer::paint(QPainter &context) const
{
	render1.paint(context);
	render2.paint(context);
}
