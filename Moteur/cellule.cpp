#include "cellule.h"

// Constructeur
Cellule::Cellule(int etat_init)
	:state(etat_init)
{}

Cellule::Cellule(const Cellule& other)
	:state(other.state)
{}
	
	
Cellule& Cellule::operator=(Cellule cell)
{
	*this= Cellule(cell);
	return *this;
}


// Setters
void Cellule::setState(int x){
	state= x;
}
