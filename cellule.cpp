#include "cellule.h"

// Constructeur
Cellule::Cellule(int etat_init, int base_pv, float coef)
	:etat(etat_init),pv(base_pv),coefficient(coef){}

Cellule::Cellule(const Cellule& other)
	:etat(other.etat), pv(other.pv),coefficient(other.coefficient){}

// Setters
void Cellule::setEtat(int x){
  this->etat = x;
}
void Cellule::setPv(int y){
  this->pv = y;
}

void Cellule::setCoeff(float z){
  this->coefficient = z;
}
