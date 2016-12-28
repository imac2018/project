#include "map.h"

Map::Map()
{

}

Position::Position(int x, int y)
	: x(x), y(y), hascode(y*10000+x)
{}

bool Position::operator<(const Position &p) const
{
	return hascode < p.hascode;
}

bool Position::operator!=(const Position &p) const
{
	return hascode != p.hascode;
}

