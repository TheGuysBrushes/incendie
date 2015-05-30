#ifndef CELLULE_H
#define CELLULE_H

class Cellule
{
	
protected:
	int state; // utile dans arbre
	
public:

// Constructors
Cellule(int etat_init=0);
Cellule(const Cellule& other);

Cellule& operator=(Cellule cell);

	// Getters and setters
	virtual int getState() const { return 0; };
	
	void setState(int x);
	
	// Other methods
	
};

#endif // CELLULE_H
