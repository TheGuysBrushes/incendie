#ifndef COORDONNEE_H
#define COORDONNEE_H

class Coordonnee
{

public:
	int col;
	int row;

public:
	Coordonnee(int _x, int _y);
	Coordonnee(const Coordonnee& other);
	virtual bool operator==(const Coordonnee& other) const;
	
};

#endif // COORDONNEE_H
