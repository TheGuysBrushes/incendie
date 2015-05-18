#ifndef ESSENCE_H
#define ESSENCE_H

#include <string>
#include <sstream>

class Essence
{
private:
	unsigned indice;
	std::string name;
	unsigned masse_V;
	float diametre;
	float hauteur;
	unsigned ageMaturite;
	bool type; // 0 pour résineux, 1 pour feuillu
	
public:
	// Constructors
	Essence(unsigned int i, std::string nom, int masse, float _diametre, float _hauteur, unsigned int age, bool t);
	Essence(const Essence& other);
	
	// Getters and setters
	unsigned	getIndice()	const {return indice;};
	std::string getName()	const {return name;};
	unsigned	getMasse()		const	{return masse_V;};
	float	getDiametre()	const {return diametre;};
	float	getHauteur()	const {return hauteur;};
	unsigned	getAgeMaturite()	const {return ageMaturite;};
	bool	getType()		const {return type;};
	
	void	setName(std::string x);
	void	setMasse(int x);
	void	setType(bool t);
	void	setHauteur(float h);
	void	setDiametre(float d);
	
	// Operateurs
	virtual Essence& operator=(const Essence& other);
	
	// Autres méthodes
	std::string toString() const;
	
};

#endif // ESSENCE_H

