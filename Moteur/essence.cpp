#include "essence.h"
#include <sstream>
#include <iostream>

using namespace std;

// Constructeurs
Essence::Essence(string nom, int masse, float _diametre, float _height, unsigned int age, bool t, unsigned int _indice):
    name(nom), masse_V(masse), diametre(_diametre), height(_height), ageMaturite(age), type(t), indice(_indice)
{}

Essence::Essence(const Essence& other):
    name(other.name), masse_V(other.masse_V), diametre(other.diametre), height(other.height),
    ageMaturite(other.ageMaturite), type(other.type), indice(other.indice)
{}

Essence::Essence(ifstream *file)
{
    char _name[50];

    file->read( (char*)&(indice),	sizeof(unsigned));
    file->read( (char*)&(masse_V),	sizeof(int));
    file->read( (char*)&(_name),		50*sizeof(char));
    file->read( (char*)&(diametre),	sizeof(float));
    file->read( (char*)&(height),	sizeof(float));
    file->read( (char*)&(ageMaturite),	sizeof(unsigned));
    file->read( (char*)&(type),	sizeof(bool));

    std::cout << "nom essence lue: "<< _name<< ";end"<< endl;

    name= (string)_name;
}

Essence::Essence(TiXmlElement *xml_essence, int _indice)
{
    //	Recuperation des valeurs de l'essence d'arbre courante
    name= xml_essence->FirstChildElement("nom")->GetText();
    masse_V= atoi(xml_essence->FirstChildElement("masse-vol")->GetText());
    // conversion hauteur et diametre en float
    istringstream(xml_essence->FirstChildElement("diametre")->GetText())>> diametre;
    istringstream(xml_essence->FirstChildElement("hauteur")->GetText())>> height;
    ageMaturite= atoi(xml_essence->FirstChildElement("age_matur")->GetText());
    type= (bool)( atoi(xml_essence->FirstChildElement("isFeuillu")->GetText()) );
    indice= _indice;
}

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
    height = h;
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
          " ; masse " << masse_V << " ; diametre : " << diametre << " ; hauteur : " << height << " age de maturite : " << ageMaturite  << " ; type " << s_type;
    return s.str();
}
