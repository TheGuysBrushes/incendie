#ifndef FORET_H
#define FORET_H

#include <vector>
#include <list>
#include <iostream>

#include "debug.h"

#include "coordonnee.h"
#include "cellule.h"
#include "arbre.h"
#include "Essence.h"
#include "vent.h"

#define MAXI 1000 

class Foret
{
private:
	int lignes;
	int colonnes;
	
	float burningCoef; // ralenti la progression du feu, fixé au lancement, peut être modifié? 
	
	Vent wind;
	std::vector< Essence > essences;
	std::vector<std::vector< Cellule* > > matrice;
	std::list< Arbre* >	onFire;
	
	// METHODES
private:
	unsigned essenceRandom(int _j, int _i, unsigned distOthers);
	
public:
	// Constructeur et destructeur
	Foret(int n_lignes, int n_colonnes, float proba=0.60, float coefFire=1.0);
	virtual ~Foret();
	
	// Initialisations
	void initEmpty();
	void randomMatrice(float probabilite);
	bool loadEssences(const std::string& fileName);
	void initialisation(float proba);
	
	// 	Getters and Setters
	int largeur(){ return colonnes;};
	int hauteur(){ return lignes;};
	std::vector< Cellule* >* operator[](int ligne) { return &(matrice[ligne]); };

	// Autres méthodes
	void allumer(int row, int col);
	void allumer(Arbre* ab);
	void enflammer(int row, int col);
	void enflammer(Arbre* ab);
	std::list< Arbre* > adjacents(int col, int row, int distance) const;
	std::list< Arbre* > adjacents(const Arbre * ab, int distance) const;
	
	// 	Avancee du temps
	// voir transition avec d'autres parametres
	void transition(Arbre* cell);
	bool NextMove();
};

#endif // FORET_H
