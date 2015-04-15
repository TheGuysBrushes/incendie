#ifndef ARBRE_H
#define ARBRE_H

#include "Essence.h"
#include "cellule.h"
#include "coordonnee.h"
#include "Essence.h"

class Arbre: public Cellule
{
private:
	Coordonnee pos;
	const Essence* essence;
	
private:
	int pv;
	int age;
	float humidite;
	float coefficient;
	
public:
	Arbre(int col, int row, const Essence* _essence, int _age = 0, unsigned int _humidite = 20);
	Arbre(Cellule* cell, int col, int row, const Essence* _essence, int _age = 0, unsigned int _humidite = 20);
	
	void initialise();
 
// 	Arbre(const Arbre& other); // attention
	virtual ~Arbre();
	
// 	virtual Arbre& operator=(const Arbre& other);
	
	
// GETTERS
	// explicites
	virtual int getEtat() const	{ return etat;};
	int getPv() const 	{ return pv;};
	float getCoeff() const 	{ return coefficient;};
	const Coordonnee& getPos() const { return pos; };
	const Essence* getEssence() const {return essence;};
	
	// implicites
	virtual bool isOnFire() { return etat==2; };
	
	
// SETTERS
	// explicites
// 	void setPv(int y);
// 	void setCoeff(float z);
	
	// implicites
	void enflammer() { etat= 2; };
	void blast() { etat= 3; };
	
	// Other Methods
	bool brule();
	
};

#endif // ARBRE_H
