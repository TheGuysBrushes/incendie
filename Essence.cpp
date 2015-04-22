#include "Essence.h"

using namespace std;
// Constructors
Essence::Essence(unsigned int i, string nom, int masse, float d, float h, unsigned int age, bool t):
indice(i),name(nom),masse_V(masse),hauteur(h),diametre(d),ageMaturite(age),type(t)
{}

Essence::Essence(const Essence& other):
indice(other.indice),name(other.name),masse_V(other.masse_V),hauteur(other.hauteur),diametre(other.diametre),
ageMaturite(other.ageMaturite),type(other.type)
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
	
	s <<  "nom :" << name << " ; type " << s_type <<
	" ; masse " << masse_V << " ; hauteur : " << hauteur << " ; diametre : " << diametre;
	return s.str();
}


