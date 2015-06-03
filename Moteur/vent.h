#ifndef VENT_H
#define VENT_H

#include <string>
#include <sstream>
#include <cmath>

#include "../debug.h"
#include "coordonnee.h"

class Vent
{
private:
// 	Coordonnee direction;
	float power_h;	// direction horizontale
	float power_v;	// direction verticale
	
public:
	Vent();
	Vent(float _power_h, float _power_v);
	Vent(const Vent& other);
// 	virtual ~Vent();
	
	// Getters and setters
	void setPower_h(float val);
	void setPower_v(float val);
	
	int getPower_h() const { return (int)power_h; };
	int getPower_v() const { return (int)power_v; };
	
	
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
