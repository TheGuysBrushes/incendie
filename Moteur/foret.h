#ifndef FORET_H
#define FORET_H

#include <vector>
#include <list>
#include <iostream>

#include "../debug.h"

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
	
	float burningCoef; // ralenti la progression du feu, il lui faut plus de tours pour avancer : affecte la précision des mesures
	
	Vent wind;
	std::vector< Essence > essences;
	std::vector<std::vector< Cellule* > > matrice;
	std::list< Arbre* >	onFire;
	// IMPROVEIT ? on pourrait faire une seule liste avec tous les arbres qui ont changé d'état
	std::list< Arbre* >	carbonized;
	std::list< Arbre* >	burned;
	
	
// METHODES
private:
	unsigned essenceRandom(int row, int col, unsigned int distOthers);
	
public:
	
	// Constructeur et destructeur
	Foret(int n_colonnes, int n_lignes, float proba = 0.60, float coefFire = 1.0);
	Foret(Foret& other, float proba=0.60);
	virtual ~Foret();
	
	// Initialisations
	void initEmpty();
	void randomMatrice(float probabilite);
	bool loadEssences(const std::string& fileName);
	void initialisation(float proba);
	void reset(int largeur, int hauteur, float coef, float proba);
	
	// 	Getters and Setters
	int largeur()	const { return colonnes;};
	int hauteur()	const { return lignes;};
	std::vector< Cellule* >* operator[](int ligne) { return &(matrice[ligne]); };
	
	const std::list< Arbre* >* getOnFire() const	{ return &onFire; };
	const std::list< Arbre* >* getCarbonized() const	{ return &carbonized; };
	const std::list< Arbre* >* getBurned() const	{ return &burned; };
	
	void clearCarbonized()	{ carbonized.clear(); };
	void clearBurned()	{ burned.clear(); };

	// Autres méthodes
	void plantTree(int row, int col);
	
	void water(Arbre* ab);
	
	void allumer(int row, int col);
	void allumer(Arbre* ab);
	
	void eteindre(Arbre* ab);
// 	void eteindre(int row, int col);
	
	void enflammer(int row, int col);
	void enflammer(Arbre* ab);
	std::list< Arbre* > adjacents(int row, int col, int distance) const;
	std::list< Arbre* > adjacents(const Arbre * ab, int distance) const;
	
	// 	Avancee du temps
	// voir transition avec d'autres parametres
	void transition(Arbre* cell);
	bool NextMove();
	
	// Affichage attributs
	void showEssences() const;
};

#endif // FORET_H
