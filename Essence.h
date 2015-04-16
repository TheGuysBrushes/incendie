#ifndef ESSENCE_H
#define ESSENCE_H

#include <string>
#include <sstream>

class Essence
{
private:
	unsigned int indice;
	std::string name;
	unsigned int masse_V;
	float hauteur;
	float diametre;
	bool type; // 0 pour résineux, 1 pour feuillu
	
public:
	// Constructors
	Essence(unsigned int i,std::string nom, int masse,float h, float d, bool t);
	Essence(const Essence& other);
	
	// Getters and setters
	std::string getName() const {return name;};
	int getMasse() const {return masse_V;};
	float getHauteur() const {return hauteur;};
	float getDiametre() const {return diametre;};
	bool getType() const {return type;};
	unsigned int getIndice() const {return indice;};
	
	void setName(std::string x);
	void setMasse(int x);
	void setType(bool t);
	void setHauteur(float h);
	void setDiametre(float d);
	
	// Operateurs
	virtual Essence& operator=(const Essence& other);
	
	// Autres méthodes
	std::string afficheEssence() const;
	
};

#endif // ESSENCE_H

