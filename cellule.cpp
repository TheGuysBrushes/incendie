#include "cellule.h"

Cellule::Cellule(int etat_init)
	:etat(etat_init)
{

}

Cellule::Cellule(const Cellule& other)
	:etat(other.etat)
{

}

int Cellule::getEtat()
{
	return etat;
}
