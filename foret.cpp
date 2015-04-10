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
		std::vector< Cellule > tmp;
		for (int j= 0; j< colonnes; ++j){
			
			int test= rand()%MAXI;
			int seuil= MAXI*(1-probabilite);
			
// 			Cellule cell= new Cellule(0);
// 			if (test>seuil){
// // 				Arbre* ab= new Arbre(*cell, Coordonnee(i, j));
// 				Cellule cell= new Cellule(1);
// 				tmp.push_back(cell);
// // 				delete(cell);
// // 				tmp.push_back(dynamic_cast <Cellule*>(ab)); //dynamic_cast <Cellule*>(ab)
// 			}
// 			else
				tmp.push_back(Cellule(test>seuil,50,0.5));
				// 			matrice[i].push_back(Cellule(0));
		}
		matrice.push_back(tmp);
	}
	
// 	depart d'incendies
	enflammer(lignes/2, colonnes/2);
	enflammer(lignes/2+1, colonnes/2);
	enflammer(lignes/2, colonnes/2 +1);
	
	enflammer(lignes/2+1, 0);
	enflammer(lignes/2+1, 1);
	
	// 	Il faudrait ecrire =Cellule(X) si il y a plusieurs arguments
}


// ###################################
//	Acces aux elements
// ################################### 


// ###################################
// 	Modification des éléments
// ###################################
void Foret::enflammer(int row, int col)
{
	Cellule& tmp= matrice[row][col];
	
	if (tmp.getEtat()==1){
		onFire.push_back(Coordonnee(col, row));
		tmp.enflammer();
	}
}

void Foret::enflammer(Coordonnee c)
{
	Cellule& tmp= matrice[c.row][c.col];
	
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
	matrice[y][x].blast();
// 	onFire.remove_if<Cellule>();
}


list< Coordonnee > Foret::adjacents(const Coordonnee& coord) const
{
	int x= coord.col;
	int y= coord.row;
	
	list< Coordonnee > liste;
	
	if (x<colonnes-1)
		liste.push_back(Coordonnee(x+1, y));
	
	if (x>0)
		liste.push_back(Coordonnee(x-1, y));
	
	if (y<lignes-1)
		liste.push_back(Coordonnee(x, y+1));
	
	if (y>0)
		liste.push_back(Coordonnee(x, y-1));
	
	return liste;
}



// ###################################
//	Avancee du temps
// ################################### 
void Foret::transition(Cellule& cell, int row, int col)
{
// 	printw("Transition %d, %d ; ", x, y); refresh();
// 	Arbre a= dynamic_cast <Arbre&>(cell);
	
// 	dynamic_cast <Arbre&>(cell).blast();
	cell.blast();
	
// 	list<Coordonnee> voisins= adjacents(dynamic_cast <Arbre&>(cell).getPos());
	list<Coordonnee> voisins= adjacents(Coordonnee(row, col));
	
	for (list<Coordonnee>::const_iterator c(voisins.begin()); c!=voisins.end(); ++c){
		// verification que la cellule n'est pas enflammer
		if(matrice[c->row][c->col].getEtat()==1)
			enflammer(*c);
	}

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
			int x= cell->col;
			int y= cell->row;
// 			try{
				transition(matrice[y][x], x, y );
// 			}
// 			catch (exception e){
// 				printw("erreur lors appel transition"); refresh();
// 			}
		}
	}
	
	return modif;
}

