#include "cellule.h"

// Constructeur
Cellule::Cellule(int etat_init)
	:etat(etat_init){}

Cellule::Cellule(const Cellule& other)
	:etat(other.etat) {}

// Setters
void Cellule::setEtat(int x){
  etat = x;
}
