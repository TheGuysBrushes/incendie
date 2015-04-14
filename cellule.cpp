#include "cellule.h"

// Constructeur
Cellule::Cellule(int etat_init)
	:etat(etat_init){}

Cellule::Cellule(const Cellule& other)
	:etat(other.etat) {}
	
	
Cellule& Cellule::operator=(Cellule cell)
{
	*this= Cellule(cell);
	return *this;
}


// Setters
void Cellule::setEtat(int x){
  etat = x;
}
