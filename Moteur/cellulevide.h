#ifndef CELLULEVIDE_H
#define CELLULEVIDE_H

#include "../debug.h"
#include "cellule.h"

class CelluleVide : public Cellule
{
public:
  int getState() const { return 0; }
};

#endif // CELLULEVIDE_H
