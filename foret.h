#ifndef FORET_H
#define FORET_H
#define MAXI 1000 

#include <vector>
#include <list>
#include <iostream>

#include "coordonnee.h"
#include "cellule.h"
#include "arbre.h"
#include "Essence.h"

class Foret
{
private:
	int lignes;
	int colonnes;
	
	std::vector<Essence> tabEss;
	std::vector<std::vector< Cellule* > > matrice;
	std::list< Arbre* >	onFire;
	
public:
	Foret(int n_lignes, int n_colonnes, float proba=0.60);
	
	void initialisation(float probabilite);
	
	// 	Acces aux elements
	int largeur(){ return colonnes;};
	int hauteur(){ return lignes;};

	std::vector< Cellule* >* operator[](int ligne) { return &(matrice[ligne]); };

	
// 	Modification des éléments
	void enflammer(int row, int col);
	void enflammer(Arbre* ab); // pour ne pas faire 2 fois l'acces à la cellule ?
	
	void eteindre(int x, int y); // a verifier si utile
	void eteindre(Arbre* ab);	// TODO
	
	
	// Fonction nécessaire pour l'initialisation des essences d'arbres
	std::vector< std::string >* explode(const std::string& str);	
	
// 	list< Arbre* > adjacents(const Coordonnee& coord) const;
	std::list< Arbre* > adjacents(const Arbre* coord) const;
	
// 	Avancee du temps
	// voir transition avec d'autres parametres
	void transition(Arbre* cell);
	bool NextMove();

};

#endif // FORET_H
