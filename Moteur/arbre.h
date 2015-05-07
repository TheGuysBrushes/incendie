#ifndef ARBRE_H
#define ARBRE_H

#include "../debug.h"

#include "essence.h"
#include "cellule.h"
#include "coordonnee.h"


class Arbre: public Cellule
{
private:
	Coordonnee pos;
	// les essences sont constantes et un arbre a toujours la même essence
	const Essence * const essence;
	
	int age;
	float humidity;
	float coefficient;
	
	int seuil;	// à partir de essence
	int hp;	// calculés lors de la création
	
public:
	// Constructeurs et destructeur
	Arbre(int col, int row, const Essence* const _essence, unsigned int _age = 0, unsigned int _humidite = 20);
	Arbre(Cellule* cell, int col, int row, const Essence*const _essence, unsigned int _age = 0, unsigned int _humidite = 20);
		
	// Getters 
	virtual int getState()	const	{ return state; };
	int getPv()	const					{ return hp;};
	float	getCoeff()	const			{ return coefficient; };
	float getHumidity()	const		{ return humidity; };
	const Coordonnee& getPos()		const	{ return pos; };
	const Essence* getEssence()	const	{ return essence; };

	// Autres méthodes
	void spark	(float coef=1.0);
	bool burn	(float coef=1.0);
	void initialise();
	
	// Méthodes constantes
	virtual bool isOnFire()	const	{ return state==2; };
	
	void water()	{ state= 1;	};
	void kindle()	{ state= 2;	};
	void blast()	{ state= -1;	};
	void uproot()	{ state= 0;	};
};

#endif // ARBRE_H
