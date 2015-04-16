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
	
	std::vector< Essence > essences;
	std::vector<std::vector< Cellule* > > matrice;
	std::list< Arbre* >	onFire;
	unsigned essence_aleatoire(int _j, int _i);
	
public:
	// Constructeur et destructeur
	Foret(int n_lignes, int n_colonnes, float proba=0.60);
	virtual ~Foret();
	
	// Initialisations
	void initEmpty();
	void randomMatrice(float probabilite); // TODO
	bool loadEssences(const std::string& fileName);
	void initialisation(float proba);
	
	// 	Getters and Setters
	int largeur(){ return colonnes;};
	int hauteur(){ return lignes;};
	std::vector< Cellule* >* operator[](int ligne) { return &(matrice[ligne]); };

	// Autres méthodes
	void enflammer(int row, int col);
	void enflammer(Arbre* ab);
	std::list< Arbre* > adjacents(int _col, int _row, int _distance) const;
	std::list< Arbre* > adjacents(const Arbre * ab, int _distance) const;
	
	// 	Avancee du temps
	// voir transition avec d'autres parametres
	void transition(Arbre* cell);
	bool NextMove();
};

#endif // FORET_H
