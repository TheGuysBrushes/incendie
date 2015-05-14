#ifndef FORET_H
#define FORET_H

#include <vector>
#include <list>
#include <iostream>
#include <fstream>

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
// 	Foret(Foret& other, float proba=0.60);
	Foret(std::string& filename);
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
	int width()	const { return colonnes; };
	int height()	const { return lignes; };
	const Vent* getVent() const { return wind; };
	std::vector< Cellule* >* operator[](int ligne) { return &(matrix[ligne]); };
	
	const std::list< Arbre* >* getOnFire() const	{ return &onFire; };
	std::list< Arbre* >* getUprooted()		{ return &uprooted; };
	std::list< Arbre* >* getExtinguished()	{ return &extinguished; };
	std::list< Arbre* >* getCarbonized()		{ return &carbonized; };
	std::list< Arbre* >* getBurned()			{ return &burned; };
	std::list< std::list< Arbre* > >* getChanged();
	
	void clearUprooted()	{ uprooted.clear(); };
	void clearExtinguished()	{ extinguished.clear(); };
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
	
	void spark(Arbre* ab, int intensite);
	void spark(int col, int row, int intensite);

	
	// Autres méthodes
	std::list< Arbre* > adjacents(int col, int row, int distance) const;
	std::list< Arbre* > adjacents(const Arbre * ab, int distance) const;
	
	
	void burnAdjacentsWind(int posCol, int posRow, int hor, int vert);
	void burnAdjacentsWind(Arbre* a, const Vent* vent);
	
	// 	Avancee du temps
	// voir transition avec d'autres parametres
	void transition(Arbre* ab); /** @deprecated */
	void transitionWind(Arbre* a, const Vent* vent);
	
	bool NextMove();
	
	// Persistance des donnees
	void loadEssences(std::ifstream* file);
	void loadMatrix(std::ifstream* file);
	
	void saveEssences(std::ofstream* file);
	void saveMatrix(std::ofstream* file);
	
	bool load(std::string fileName= "save_forest");
	bool save(std::string fileName= "save_forest");
	
	
	// Affichage attributs
	void showEssences() const;
	void cut(int xDep, int yDep, int xArr, int yArr);
	void delay(int xDep, int yDep, int xArr, int yArr);
};

#endif // FORET_H
