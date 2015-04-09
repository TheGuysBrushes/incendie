#include "foret.h"

#include <cstdlib>
#include <ctime>
#include <curses.h> // temp
// #include <bits/stl_algo.h>
// #include <bits/algorithmfwd.h>

using namespace std;

Foret::Foret(int n_lignes, int n_colonnes)
	: lignes(n_lignes), colonnes(n_colonnes)
{
	initialisation(0.65);
}

/**
 * Initialise la matrice
 * @param probabilite chance qu'a un arbre d'etre place sur chaque case
 */
void Foret::initialisation(float probabilite)
{
	srand(std::time(0));
	if (probabilite>1){
		probabilite=0.6;
		cout << "MIS A DEFAUT"<< endl;
	}
	
	for (int i= 0; i< lignes; ++i){
		std::vector<Cellule> tmp;
		for (int j= 0; j< colonnes; ++j){
			
			int test= rand()%MAXI;
			int seuil= MAXI*(1-probabilite);
			
			// 		utilise un booléen pour initialiser
				tmp.push_back(Cellule(test>seuil));
				
				// 			matrice[i].push_back(Cellule(0));
		}
		matrice.push_back(tmp);
	}
	
	
	enflammer(lignes/2, 0);
	enflammer(lignes/2+1, 1);
	
	// 	Il faudrait ecrire =Cellule(X) si il y a plusieurs arguments
	// 	matrice[lignes/2][colonnes/2]= Cellule(1);
	
	// 	matrice[0][colonnes/2]= 3;
}


// ###################################
// 	Modification des éléments
// ###################################
void Foret::enflammer(int x, int y)
{
	Cellule& tmp= matrice[x][y];
	
	if (tmp.getEtat()==1){
		onFire.push_back(Coordonnee(x, y));
		tmp.enflammer();
	}
}

void Foret::enflammer(Coordonnee c)
{
	Cellule& tmp= matrice[c.x][c.y];
	
	if (tmp.getEtat()==1){
		onFire.push_back(c);
		tmp.enflammer();
	}
}


void Foret::enflammer(Cellule& cell, int x, int y)
{
	cell.enflammer();
	onFire.push_back(Coordonnee(x, y));
}


void Foret::eteindre(int x, int y)
{
	matrice[x][y].blast();
// 	onFire.remove_if<Cellule>();
}



// ###################################
//	Avancee du temps
// ################################### 
void Foret::transition(Cellule& cell, int x, int y)
{
// 	printw("Transition %d, %d ; ", x, y); refresh();
// 	if (cell.isOnFire()) {
		cell.blast();
// 			return true;
		
		list<Coordonnee> voisins= adjacents(Coordonnee(x, y));
		for (list<Coordonnee>::const_iterator c(voisins.begin()); c!=voisins.end(); ++c){
			enflammer(*c);
		}
		
// 	}
	
// 		return false;
}


bool Foret::NextMove()
{
	bool modif= false;
	
// 		for_each<Cellule>(matrice[i].begin(), matrice[i].end(), transition);
// 	for (int i= 0; i< lignes; ++i)
	
	if (!onFire.empty()){
		modif= true;
		
		list<Coordonnee> old= onFire;
		onFire.clear();
	
		// TODO utiliser mapping
		for (list<Coordonnee>::const_iterator cell(old.begin()); cell!=old.end(); ++cell){
			int x= cell->x;
			int y= cell->y;
			transition(matrice[x][y], x, y);
		}
	}
	
	return modif;
}



// ###################################
//	Acces aux elements
// ################################### 
vector< Cellule > Foret::operator[](int x)
{
	return matrice[x];
}


list< Coordonnee > Foret::adjacents(const Coordonnee& coord) const
{
	int x= coord.x;
	int y= coord.y;
	
	list< Coordonnee > liste;
	if (x<colonnes-1)
		liste.push_back(Coordonnee(x+1, y));
	
	if (x>0)
		liste.push_back(Coordonnee(x-1, y));

	if (y<lignes-1){
		liste.push_back(Coordonnee(x, y+1));
// 		printw("y : %d  ;  ", y); refresh();
	}
	
	if (y>0)
		liste.push_back(Coordonnee(x, y-1));
	
	return liste;
}
