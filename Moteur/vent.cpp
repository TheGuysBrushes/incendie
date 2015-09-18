#include "vent.h"

using namespace std;
Vent::Vent(){

}

Vent::Vent(float _power_h, float _power_v)
	:power_h(_power_h),power_v(_power_v)
{
}

Vent::Vent(const Vent& other)
	: power_h(other.power_h), power_v(other.power_v)
{
}

Vent::~Vent(){}

void Vent::setPower_h(float val)
{
	power_h = val;
}

void Vent::setPower_v(float val)
{
	power_v = val;
}


// ####################
// 	OPERATIONS
// ####################

Vent& Vent::operator+(const Vent& other)
{
	return *(new Vent(power_v + other.power_v, power_h + other.power_h));
}

Vent& Vent::operator-(const Vent& other)
{
	return *(new Vent(power_v - other.power_v, power_h - other.power_h));
}

Vent& Vent::operator*(float coefficient)
{
	return *(new Vent(power_v*coefficient, power_h*coefficient));
}

Vent& Vent::operator/(float coefficient)
{
	return *(new Vent(power_v/coefficient, power_h/coefficient));
}



// ####################
// 	ASSIGNATIONS
// ####################

Vent& Vent::operator=(const Vent& other)
{
	return *this;
}


Vent& Vent::operator+=(const Vent& other)
{
		power_v += other.power_v;
		power_h += other.power_h;
		
		return *this;
}

std::string Vent::toString() const
{
	ostringstream os;

	os << "(Force Nord : "<< power_v<< " Force Est : "<< power_h << ")";
	return os.str();
}

int Vent::getSpeed() const
{
	return sqrt( (power_v*power_v) + (power_h*power_h) ) *50.0;
}


// ####################
// 	AUTRES CLASSES
// ####################
ostream& operator<<(ostream& os, const Vent& vent)
{
	os<< vent.toString();
	return os;
}
