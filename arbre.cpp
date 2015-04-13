#include "arbre.h"

Arbre::Arbre(int col, int row, int base_pv, float coef)
	: Cellule(1), pos(col, row), pv(base_pv), coefficient(coef)
{

}


Arbre::Arbre(Cellule* cell, int col, int row, int base_pv, float coef)
: Cellule (*cell), pos(col, row), pv(base_pv), coefficient(coef)
{
// 	delete(cell);
}

Arbre::Arbre(const Arbre& other)
: Cellule(1) , pos(other.pos), pv(other.getPv()), coefficient(other.getCoeff())
{

}

Arbre::~Arbre()
{

}

Arbre& Arbre::operator=(const Arbre& other)
{
return *this;
}


void Arbre::setPv(int y){
	pv = y;
}

void Arbre::setCoeff(float z){
	coefficient = z;
}
