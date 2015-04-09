#ifndef CELLULE_H
#define CELLULE_H

class Cellule
{

private:
	int etat;
	
public:

Cellule(int etat_init);
Cellule(const Cellule& other);

	int getEtat();
	virtual bool isTree() { return etat>0; };
	virtual	bool isOnFire() { return etat==2; };
	
	virtual void enflammer() { etat= 2; };
	virtual void blast() { etat= 3; };
	
// 	Cellule& operator=(Cellule cell);
	
};

#endif // CELLULE_H
