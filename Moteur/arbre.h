#ifndef ARBRE_H
#define ARBRE_H

#include "../debug.h"

#include "Essence.h"
#include "cellule.h"
#include "coordonnee.h"


class Arbre: public Cellule
{
private:
	Coordonnee pos;
	// les essences sont constantes et un arbre a toujours la même essence
	const Essence * const essence;
	
	int age;
	float humidite;
	float coefficient;
	
	int seuil;	// à partir de essence
	int pv;	// calculés lors de la création
	
public:
	// Constructeurs et destructeur
	Arbre(int col, int row, const Essence* const _essence, unsigned int _age = 0, unsigned int _humidite = 20);
	Arbre(Cellule* cell, int col, int row, const Essence*const _essence, unsigned int _age = 0, unsigned int _humidite = 20);
		
	// Getters 
	virtual int getEtat()	const	{ return etat; };
	int getPv()	const					{ return pv;};
	float	getCoeff()	const			{ return coefficient; };
	float getHumidite()	const		{ return humidite; };
	const Coordonnee& getPos()		const	{ return pos; };
	const Essence* getEssence()	const	{ return essence; };

	// Autres méthodes
	void enflammer	(float coef=1.0);
	bool brule		(float coef=1.0);
	void initialise();
	
	// Méthodes constantes
	virtual bool isOnFire()	const	{ return etat==2; };
	
	void water()	{ etat= 1;	};
	void burn()		{ etat= 2;	};
	void blast()	{ etat= 3;	};
};

#endif // ARBRE_H
