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
	bool isOnFire() { return etat==2; };
	
	void enflammer() { etat= 2; };
	void blast() { etat= 3; };
	
};

#endif // CELLULE_H
