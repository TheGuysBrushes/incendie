#ifndef VENT_H
#define VENT_H

#include <string>
#include <sstream>

#include "../debug.h"

#include "coordonnee.h"


class Vent
{
private:
// 	Coordonnee direction;
	float power_h;	// NS= Nord-Sud
	float power_v;	// EW= Est-Ouest
	
public:
	Vent(float _power_h, float _power_v);
	Vent(const Vent& other);
// 	virtual ~Vent();
	
	void setPower_h(float val);
	void setPower_v(float val);
	
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
