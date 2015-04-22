#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdlib.h>
#include <ncurses.h>
#include <vector>

// #include "cellule.h"
#include "foret.h"

class Terminal
{

private:
	Foret foret;
	long temps;
	char b_taille; // si vrai, divise la taille hztl d'une case par 2
	
public:
	Terminal(int hauteur, int largeur, float proba, long int nTemps = 0.60, float coef_brulure=1.0, bool t = 0);
	
	void end();
	void afficheContour(int largeur);
	
	void afficheCase(const Cellule* cell);
	void afficheLigne(int num_ligne);
	void afficheForet();
	
	void next();
	void run();
	
	int main_terminal(int argc, char** argv);
	
};

#endif // TERMINAL_H
