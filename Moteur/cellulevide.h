#ifndef CELLULEVIDE_H
#define CELLULEVIDE_H

#include "../debug.h"
#include "cellule.h"

class CelluleVide : public Cellule
{
public:
  CelluleVide();
  CelluleVide(const CelluleVide& other);
  virtual ~CelluleVide();
  
  int getState() const { return 0; };
};

#endif // CELLULEVIDE_H
