#include "arbre.h"
#include <math.h>
Arbre::Arbre(int col, int row, const Essence* _essence, int base_pv, int a, float h, float coef)
	: Cellule(1), pos(col, row),essence(_essence), pv(base_pv),age(a),humidite(h), coefficient(coef)
{
	initialise();
}

// attention risque de créer des incohérences
Arbre::Arbre(Cellule* cell, int col, int row, const Essence* _essence, int base_pv, int a, float h, float coef)
: Cellule (*cell), pos(col, row), pv(base_pv),age(a),humidite(h), coefficient(coef)
{
	initialise();
}

Arbre::Arbre(const Arbre& other)
: Cellule(1) , pos(other.pos), pv(other.pv),age(other.age),humidite(other.humidite), coefficient(other.coefficient)
{
	initialise();
}

Arbre::~Arbre()
{

}

Arbre& Arbre::operator=(const Arbre& other)
{
	*this= Arbre(other);
	return *this;
}


void Arbre::setPv(int y){
	pv = y;
}

void Arbre::setCoeff(float z){
	coefficient = z;
}

void Arbre::initialise()
{
	// Initialisation des points de vie de l'arbre
	int rayon = this->essence->getDiametre() / 2;
	int hauteur = this->essence->getHauteur();
	int points = 4/3*M_PI*rayon*rayon*hauteur*this->essence->getMasse()/100;
	// Les données des essences sont décrites pour un arbre de 50 ans à 12% d'humidité
	pv = points * age/50;
	pv *= (1+humidite);
	
	// L'initialisation du coefficient sera faite plus tard puisqu'il dépend des paramètres extérieurs à l'arbre 
	// Humidité ambiante, force du vent etc
	this->coefficient = 0.5;
	
}
