#include "coordonnee.h"

Coordonnee::Coordonnee(int _x, int _y)
	:x(_x), y(_y)
{

}

Coordonnee::Coordonnee(const Coordonnee& other)
	:x(other.x), y(other.y)
{

}

bool Coordonnee::operator==(const Coordonnee& other) const
{
	return (other.x==x && other.y==y);
}

