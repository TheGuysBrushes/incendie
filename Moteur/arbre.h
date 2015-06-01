#ifndef ARBRE_H
#define ARBRE_H

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "../debug.h"
#include "coordonnee.h"
#include "cellule.h"
#include "essence.h"

class Arbre: public Cellule
{
private:
	Coordonnee pos;
	// les essences sont constantes et un arbre a toujours la même essence
	const Essence * const essence;
	
// 	int age;
	float humidity;
	float coefficient;
	int hp;	// calculés lors de la création
	
public:
	// Constructeurs et destructeur
	Arbre(int col, int row, const Essence*const _essence, unsigned int _age = 0, unsigned int _humidite = 20, int _coefficient = 1);
	Arbre(Cellule* cell, int col, int row, const Essence* const _essence, unsigned int age = 0, unsigned int _humidite = 20, int _coefficient = 1);
	Arbre(int col, int row, Cellule* cell, const Essence* const _essence, unsigned int _humidite, int _hp, int _coefficient = 1);
		
	// Getters 
	virtual int getState()	const	{ return state; };
	int getPv()	const					{ return hp;};
	float	getCoeff()	const			{ return coefficient; };
	float getHumidity()	const		{ return humidity; };
	const Coordonnee& getPos()		const	{ return pos; };
	const Essence* getEssence()	const	{ return essence; };
	
	// Setters
	void setCoefficient(float x);

	// Autres méthodes
	/**
	 * L'arbre reçoit des flammes d'un voisin et risque de s'enflammer
	 * @author Florian
	 * @param coefTransmission force de transmisson de l'arbre en feu au voisin, correspond à un rapport (force du vent)/(dist à l'arbre en feu)
	 */
	void spark	(float coefTransmission = 1.0);
	bool burn	(float coefBrulure = 1.0);
	void initialise(unsigned age);
	
	// Méthodes constantes
	virtual bool isOnFire()	const	{ return state==2; };
	
	void delay(float coefRalentissement)	/*{ state= 1;	}*/;
	void kindle()	{ state= 2;	};
	void blast()	{ state= -1; };
	void uproot()	{ state= -2; };
};

#endif // ARBRE_H
