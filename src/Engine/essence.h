#ifndef ESSENCE_H
#define ESSENCE_H

#include <string>
#include <fstream>
#include "TinyXML/tinyxml.h"

class Essence
{
private:
    std::string name;
    unsigned masse_V;
    float diametre;
    float height;
    unsigned ageMaturite;
    bool type; // 0 pour résineux, 1 pour feuillu
    unsigned indice;

public:
    // Constructors
    Essence(std::string nom, int masse, float _diametre, float _hauteur, unsigned int age, bool t, unsigned int i);
    /**
     * Construit une essence à partir des valeurs lues dans un flux
     * @param file flux, tête de lecture placée au début des valeurs d'une essence
     */
    Essence(std::ifstream* file);
    /**
     * Construit une essence à partir d'un élément xml
     * @param xml_essence élément xml contenant les valeur d'une essence
     */
    Essence(TiXmlElement * xml_essence, int _indice = 0);
    Essence(const Essence& other);

    // Getters and setters
    unsigned	getIndice()	const {return indice;}
    std::string getName()	const {return name;}
    unsigned	getMasse()		const	{return masse_V;}
    float	getDiametre()	const {return diametre;}
    float	getHeight()	const {return height;}
    unsigned	getAgeMaturite()	const {return ageMaturite;}
    bool	getType()		const {return type;}

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

bool tryLoadEssences(const std::string& fileName);
#endif // ESSENCE_H

