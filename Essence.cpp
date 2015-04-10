#include "Essence.h"

Essence::Essence(string nom, int masse, bool t):name(nom),masse_V(masse),type(t){
}

Essence::Essence(const Essence& other):name(other.name),masse_V(other.masse_V),type(other.type){
}

Essence& Essence::operator=(const Essence& other)
{
    return *this;
}

void Essence::setMasse(int x){
  masse_V = x;
}

void Essence::setName(string x){
  name = x;
}

void Essence::setType(bool t){
  type = t;
}

