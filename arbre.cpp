#include "arbre.h"

#include <cmath>
#include <iostream>

using namespace std;

Arbre::Arbre(int col, int row, const Essence* _essence, unsigned _age, unsigned _humidite)
	: Cellule(1), pos(col, row),essence(_essence),age(_age),humidite(_humidite)
{	
	initialise();
}

// attention risque de créer des incohérences
Arbre::Arbre(Cellule* cell, int col, int row, const Essence* _essence, unsigned _age, unsigned _humidite)
: Cellule (*cell), pos(col, row),age(age),humidite(humidite)
{
	initialise();
// 	delete(cell);
}


void Arbre::initialise()
{
	// Initialisation des points de vie de l'arbre
	float rayon = essence->getDiametre() / 2.0;
	float	hauteur = essence->getHauteur();
	
	float masseV= essence->getMasse()/1000.0;
	unsigned points = (unsigned) ( 4.0/3.0* (M_PI* (rayon*rayon)*hauteur) * masseV );
	
	// Les données des essences sont décrites pour un arbre de 50 ans à 12% d'humidité
	pv = points * age/50.0;
	pv *= 1+ humidite/100.0;
// 	cout << " pv : "<< pv << endl;
	
	// L'initialisation du coefficient sera faite plus tard puisqu'il dépend des paramètres extérieurs à l'arbre 
	// Humidité ambiante, force du vent etc
// 	coefficient = 0.5;
	
}

// Arbre::Arbre(const Arbre& other)
// : Cellule(1) , pos(other.pos), pv(other.pv),age(other.age),humidite(other.humidite), coefficient(other.coefficient)
// {
// 
// }

Arbre::~Arbre()
{

}

// Arbre& Arbre::operator=(const Arbre& other)
// {
// 	*this= Arbre(other);
// 	return *this;
// }


// void Arbre::setPv(int y){
// 	pv = y;
// }
// 
// void Arbre::setCoeff(float z){
// 	coefficient = z;
// }

bool Arbre::brule()
{
	pv-= 45; // TODO : celon coef, paramètres ... (45 est arbitraire)
	if (pv<=0){
		blast();
		return false;
	}
	else return true;
}
