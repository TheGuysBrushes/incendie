#ifndef ARBRE_H
#define ARBRE_H

#include "Essence.h"
#include "cellule.h"
#include "coordonnee.h"

class Arbre: public Cellule
{
private:
	Coordonnee pos;
	
private:
	int pv;
	float coefficient;
	
public:
	Arbre(int col, int row, int base_pv = 0, float coef = 0.0);
	Arbre(Cellule* cell, int col, int row, int base_pv = 0, float coef = 0.0);
  Arbre(const Arbre& other);
  virtual ~Arbre();
  virtual Arbre& operator=(const Arbre& other);
	
// 	getters
	virtual int getEtat() const {return etat;};
	int getPv() const {return pv;};
	float getCoeff() const {return coefficient;};
	const Coordonnee& getPos() const { return pos; };
	
// 	setters
	void setPv(int y);
	void setCoeff(float z);
	
	virtual void enflammer() { etat= 2; };
	virtual void blast() { etat= 3; };
	
};

#endif // ARBRE_H