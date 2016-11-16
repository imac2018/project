#include "mode.h"

Mode::Mode() : _renderer(NULL)
{}

const Renderer *Mode::renderer() const
{
	return _renderer;
}
