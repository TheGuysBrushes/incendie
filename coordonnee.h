#ifndef COORDONNEE_H
#define COORDONNEE_H

class Coordonnee
{

public: //utile de mettre en proteger
	int x;
	int y;
	
public:
Coordonnee(int _x, int _y);
Coordonnee(const Coordonnee& other);
virtual bool operator==(const Coordonnee& other) const;
};

#endif // COORDONNEE_H
