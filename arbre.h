#ifndef ARBRE_H
#define ARBRE_H

#include "debug.h"

#include "Essence.h"
#include "cellule.h"
#include "coordonnee.h"


class Arbre: public Cellule
{
private:
	Coordonnee pos;
	const Essence* essence;
	
	int pv;
	int age;
	float humidite;
	float coefficient;
	
public:
	// Constructeurs et destructeur
	Arbre(int col, int row, const Essence* _essence, unsigned _age = 0, unsigned _humidite = 20);
	Arbre(Cellule* cell, int col, int row, const Essence* _essence, unsigned _age = 0, unsigned _humidite = 20);
	virtual ~Arbre();	
		
	// Getters 
	virtual int getEtat() const	{ return etat; };
	int getPv() const		{ return pv;};
	float getCoeff() const	{ return coefficient; };
	float getHumidite() const 	{ return humidite; };
	const Coordonnee& getPos()		const { return pos; };
	const Essence* getEssence()	const { return essence; };

	// Autres méthodes
	bool brule();
	void initialise();
	
	// Méthodes constantes
	virtual bool isOnFire() { return etat==2; };
	
	void enflammer() { etat= 2; };
	void blast() { etat= 3; };
};

#endif // ARBRE_H
