#pragma once
#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm> /// @SEE Normalement inutile, permet de ne pas utilise -lm lors compilation

class Coordonnee
{

public:
	int col;
	int row;

public:
    Coordonnee(int _x, int _y);
    Coordonnee(const Coordonnee& other);
    virtual bool operator==(const Coordonnee& other) const;

    // Affichage
	std::ostream& print(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Coordonnee& coord)
	    { return coord.print(out); }
	
    // Calculs
	/**
	 * Calcul de la distance Manhattan entre deux points
	 * @return distance manhattan : entier non signé
	 */
    unsigned manhattanDistance(const Coordonnee& other) const;
	/**
	 * Calcul de la distance euclidienne entre deux points
	 * @return distance euclidienne : flottant non signé
	 */
    float euclideanDistance(const Coordonnee& other) const;
	
};

#endif // COORDINATES_H
