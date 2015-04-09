#ifndef FORET_H
#define FORET_H
#define MAXI 1000 

#include <vector>
#include <list>
#include <iostream>

#include "coordonnee.h"
#include "cellule.h"

class Foret
{
private:
	int lignes;
	int colonnes;
	std::vector<std::vector< Cellule> > matrice;
	std::list<Coordonnee>	onFire;
	
public:
	Foret(int n_lignes, int n_colonnes);
	
	void initialisation(float probabilite);
	
	
// 	Modification des éléments
	void enflammer(int x, int y);
	void enflammer(Coordonnee c);
	void enflammer(Cellule& cell, int x, int y); // pour ne pas faire 2 fois l'acces à la cellule ?
	
	void eteindre(int x, int y);
	
// 	Avancee du temps
	// voir transition avec d'autres parametres
	void transition(Cellule& cell, int x, int y);
	bool NextMove();
	
// 	Acces aux elements
	int largeur(){ return colonnes;};
	int hauteur(){ return lignes;};
	std::vector<Cellule> operator[](int x);
	
	std::list< Coordonnee > adjacents(const Coordonnee& coord) const;
};

#endif // FORET_H
