#ifndef FORET_H
#define FORET_H
#define MAXI 1000 

#include <vector>
#include <iostream>
#include "cellule.h"

class Foret
{
private:
	int lignes;
	int colonnes;
	
public: // temporaire
	std::vector<std::vector<Cellule> > matrice;
	
public:
	Foret(int n_lignes, int n_colonnes);
	int largeur(){ return colonnes;};
	int hauteur(){ return lignes;};
	
	void initialisation(float probabilite);
	
	void transition(Cellule cell);
	
	bool NextMove();
	
// 	operator[] ;
};

#endif // FORET_H
