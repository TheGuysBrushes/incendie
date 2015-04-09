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
	
public:
	Terminal(int hauteur, int largeur);
		
	
	void end();
	void afficheContour(int largeur);
	
	void afficheCase(Cellule* cell);
	void afficheLigne(int num_ligne);
	void afficheForet();
	
	void next();
	void run();
	
};

#endif // TERMINAL_H
