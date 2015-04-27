#include "coordonnee.h"

Coordonnee::Coordonnee(int _x, int _y)
	:col(_x), row(_y)
{

}

Coordonnee::Coordonnee(const Coordonnee& other)
	:col(other.col), row(other.row)
{

}

bool Coordonnee::operator==(const Coordonnee& other) const
{
	return (other.col==col && other.row==row);
}

