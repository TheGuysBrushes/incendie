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
	int state;
	Coordonnee pos;
	// les essences sont constantes et un arbre a toujours la même essence
	const Essence * const essence;
	
// 	int age;
	float humidity;
	float coefficient;
	int hp;	// calculés lors de la création
	
public:
	
// Setters
	void setState(int x);
  
// Constructeurs et destructeur
	/**
	 * Construit un arbre non enflammé à partir d'une position
	 * @param age age de l'arbre, permet de calculer ses PV
	 * @param all paramètres d'initialisation des attributs de l'arbre
	 */
	Arbre(int col, int row, const Essence*const _essence, unsigned int _age = 0, unsigned int _humidite = 20, int _coefficient = 1);
	/**
	 * Construit un arbre non enflammé "à partir" d'une cellule
	 * @param cell cellule à remplacer, elle est désallouée
	 * @param age age de l'arbre, permet de calculer ses PV
	 * @param all paramètres d'initialisation des attributs de l'arbre
	 */
	Arbre(Cellule* cell, int col, int row, const Essence* const _essence, unsigned int age = 0, unsigned int _humidite = 20, int _coefficient = 1);
	/**
	 * Construit un arbre non enflammé par dessus une cellule, en donnant des PV plutot que de les calculer avec l'age
	 * @param cell cellule à remplacer, elle est désallouée
	 * @param all paramètres d'initialisation des attributs de l'arbre
	 */
	Arbre(int col, int row, Cellule* cell, const Essence* const _essence, unsigned int _humidite, int _hp, int _coefficient = 1);
	
	
	
	/**
	 * Initialise les points de vie de l'arbre en fonction des caractères de son essence
	 * et de ses propriétés discrètes
	 * @author Ugo Florian
	 */
	void initialise(unsigned age);
		
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
	/**
	 * Retire un nombre de points de vie à l'arbre
	 * @author Florian
	 * @param coef
	 */
	bool burn	(float coefBrulure = 1.0);
	
	// Méthodes constantes
	virtual bool isOnFire()	const	{ return state==2; };
	/**
	 * Modifie la vitesse de transmisson du feu (multiplié)
	 * @author Ugo
	 */
	void delay(float coefRalentissement);
	void kindle()	{ state= 2;	};
	void blast()	{ state= -1; };
	void uproot()	{ state= -2; };
};

#endif // ARBRE_H
