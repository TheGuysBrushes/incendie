#ifndef CELLULE_H
#define CELLULE_H

class Cellule
{
	
protected:
	int etat; // utilie dans arbre
	
public:

	// Constructors
	Cellule(int etat_init=0);
	Cellule(const Cellule& other);
	
	Cellule& operator=(Cellule cell);
	
		// Getters and setters
		virtual int getEtat() const { return 0; };
		
		void setEtat(int x);
		
		// Other methods
	
};

#endif // CELLULE_H
