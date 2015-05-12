#ifndef FORET_H
#define FORET_H

#include <vector>
#include <list>
#include <iostream>

#include "../debug.h"

#include "cellule.h"
#include "arbre.h"
#include "essence.h"
#include "vent.h"

#define MAXI 1000 

class Foret
{
private:
	int lignes;
	int colonnes;
	
	float burningCoef; // ralenti la progression du feu, il lui faut plus de tours pour avancer : affecte la précision des mesures
	
	Vent* wind;
	std::vector< Essence > essences;
	std::vector<std::vector< Cellule* > > matrix;
	std::list< Arbre* >	onFire;
	// IMPROVEIT ? on pourrait faire une seule liste avec tous les arbres qui ont changé d'état
	/* IMPROVEIT ou TODO remove it : "Pour l'instant" Une liste par état ; les listes représentent les nouveaux éléments dans cet état */
	std::list< Arbre* >	uprooted;
	std::list< Arbre* >	extinguished;
	std::list< Arbre* >	burned;
	std::list< Arbre* >	carbonized;
	
	
// METHODES
private:
	unsigned essenceRandom(int col, int row, unsigned int distOthers);
	
public:
	
	// Constructeur et destructeur
	Foret(int n_colonnes, int n_lignes, float proba = 0.60, float coefFire = 1.0);
	Foret(Foret& other, float proba=0.60);
	virtual ~Foret();
	
	// Initialisations
	void initEmpty();
	void randomMatrix(float probabilite);
	bool loadEssences(const std::string& fileName);
	void initialisation(float proba);
	
	void clean();

	
	void setValues(int largeur, int hauteur, float coef);
	void setWind(int EO, int NS);
	
	// 	Getters and Setters
	int width()	const { return colonnes;};
	int height()	const { return lignes;};
	const Vent* getVent() const { return wind; };
	std::vector< Cellule* >* operator[](int ligne) { return &(matrix[ligne]); };
	
	const std::list< Arbre* >* getOnFire() const	{ return &onFire; };
	std::list< Arbre* >* getUprooted()		{ return &uprooted; };
	std::list< Arbre* >* getExtinguished()	{ return &extinguished; };
	std::list< Arbre* >* getCarbonized()		{ return &carbonized; };
	std::list< Arbre* >* getBurned()			{ return &burned; };
	std::list< std::list< Arbre* > >* getChanged();
	
	void clearCarbonized()	{ carbonized.clear(); };
	void clearBurned()	{ burned.clear(); };
	
	// Manipulations d'arbre
	void plantTree(int col, int row);

	void uproot(Arbre* ab);
	void uproot(int col, int row);
	
	void water(Arbre* ab);

	void kindle(Arbre* ab);
	void kindle(int col, int row);
	
	void blast(Arbre* ab);
// 	void eteindre(int row, int col);
	
	void spark(Arbre* ab);
	void spark(int col, int row);

	
	// Autres méthodes
	std::list< Arbre* > adjacents(int col, int row, int distance) const;
	std::list< Arbre* > adjacents(const Arbre * ab, int distance) const;
	
	void burnAdjacentsWind(Arbre* a);
	
	// 	Avancee du temps
	// voir transition avec d'autres parametres
	void transition(Arbre* cell); /** @deprecated */
	void transitionWind(Arbre* cell);
	
	bool NextMove();
	
	// Affichage attributs
	void showEssences() const;
};

#endif // FORET_H
