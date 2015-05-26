#include "essence.h"

using namespace std;

// Constructeurs
Essence::Essence(unsigned int i, string nom, int masse, float _diametre, float _hauteur, unsigned int age, bool t):
indice(i), name(nom), masse_V(masse), diametre(_diametre), hauteur(_hauteur), ageMaturite(age), type(t)
{}

Essence::Essence(const Essence& other):
indice(other.indice), name(other.name), masse_V(other.masse_V), diametre(other.diametre), hauteur(other.hauteur), ageMaturite(other.ageMaturite), type(other.type)
{}

Essence& Essence::operator=(const Essence& other)
{
	*this= Essence(other);
	return *this;
}
// Getters and Setters
void Essence::setMasse(int x){
	masse_V = x;
}

void Essence::setName(string x){
	name = x;
}

void Essence::setType(bool t){
	type = t;
}
void Essence::setHauteur(float h)
{
	hauteur = h;
}

void Essence::setDiametre(float d)
{
	diametre = d;
}

// Autres méthodes
string Essence::toString() const
{
	ostringstream s;
	
	string s_type("Epineux");
	if (type)
		s_type= "Feuillu";
	
	s <<  "nom :" << name <<
	" ; masse " << masse_V << " ; diametre : " << diametre << " ; hauteur : " << hauteur << " age de maturite : " << ageMaturite  << " ; type " << s_type;
	return s.str();
}


