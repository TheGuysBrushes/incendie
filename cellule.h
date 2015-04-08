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
};

#endif // CELLULE_H
