#ifndef CELLULE_H
#define CELLULE_H

class Cellule
{
	
public:

// Constructors
// Cellule();
// Cellule(const Cellule& other);
virtual ~Cellule() =0;

// Cellule& operator=(Cellule cell);

	// Getters and setters
	virtual int getState() const =0;
	
	// Other methods
	
};

#endif // CELLULE_H
