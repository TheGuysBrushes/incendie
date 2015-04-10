#ifndef ARBRE_H
#define ARBRE_H

#include "Essence.h"

class Arbre 
{
private:
 
public:
  Arbre();
  Arbre(const Arbre& other);
  virtual ~Arbre();
  virtual Arbre& operator=(const Arbre& other);
};

#endif // ARBRE_H
