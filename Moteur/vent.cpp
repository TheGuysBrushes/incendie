#include "vent.h"
#include <cmath>

using namespace std;

Vent::Vent(float NordSud, float EstOuest)
	:NS(NordSud) , EW(EstOuest)
{
}

Vent::Vent(const Vent& other)
	: NS(other.NS), EW(other.EW)
{
}

// Vent::~Vent(){}


// ####################
// 	OPERATIONS
// ####################

Vent& Vent::operator+(const Vent& other)
{
	return *(new Vent(NS + other.NS, EW + other.EW));
}

Vent& Vent::operator-(const Vent& other)
{
	return *(new Vent(NS - other.NS, EW - other.EW));
}

Vent& Vent::operator*(float coefficient)
{
	return *(new Vent(NS*coefficient, EW*coefficient));
}

Vent& Vent::operator/(float coefficient)
{
	return *(new Vent(NS/coefficient, EW/coefficient));
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
		NS += other.NS;
		EW += other.EW;
		
		return *this;
}

std::string Vent::toString() const
{
	ostringstream os;

	os << "(Force Nord : "<< NS<< " Force Est : "<< EW << ")";
	return os.str();
}

int Vent::getSpeed() const
{
	return sqrt( (NS*NS) + (EW*EW) ) *50.0;
}


// ####################
// 	AUTRES CLASSES
// ####################
ostream& operator<<(ostream& os, const Vent& vent)
{
	os<< vent.toString();
	return os;
}
