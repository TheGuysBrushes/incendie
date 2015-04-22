#ifndef VENT_H
#define VENT_H

#include <string>
#include <sstream>

#include "debug.h"

#include "coordonnee.h"


class Vent
{
private:
// 	Coordonnee direction;
	float NS;	// NS= Nord-Sud
	float EW;	// EW= Est-Ouest
	
public:
	Vent(float NordSud, float EstOuest);
	Vent(const Vent& other);
// 	virtual ~Vent();
	
	void setNS(float val);
	void setEW(float val);
	void setVent(float _NS, float _EW); // TODO trouver meilleur nom pour dire qu'on redéfinit direction et puissance
	
	virtual Vent& operator+(const Vent& other);
	virtual Vent& operator-(const Vent& other);
	virtual Vent& operator*(float coefficient);
	virtual Vent& operator/(float coefficient);
	
	virtual Vent& operator=(const Vent& other);
	virtual Vent& operator+=(const Vent& other);
	
	std::string toString() const;
	
	int getSpeed() const;
// 	std::ostringstream os;
	
};

std::ostream& operator<<(std::ostream& os, const Vent& vent);

#endif // VENT_H
