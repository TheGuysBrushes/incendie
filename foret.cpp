#include "foret.h"

#include <cstdlib>
#include <bits/stl_algo.h>
// #include <bits/algorithmfwd.h>

using namespace std;

Foret::Foret(int n_lignes, int n_colonnes)
	: lignes(n_lignes), colonnes(n_colonnes)
{
	initialisation(0.55);
}

/**
 * Initialise la matrice
 * @param probabilite chance qu'a un arbre d'etre place sur chaque case
 */
void Foret::initialisation(float probabilite)
{
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
	
	matrice[lignes/2][0]= 2;
	matrice[lignes/2+1][1]= 2;
	
	// 	Il faudrait ecrire =Cellule(X) si il y a plusieurs arguments
	// 	matrice[lignes/2][colonnes/2]= Cellule(1);
	
	// 	matrice[0][colonnes/2]= 3;
}


void Foret::transition(Cellule cell)
{
	if (cell.getEtat()==2){
// 				if ((j-1)!= matrice[i].begin())
// 					*(j-1)= Cellule(2);
			
			cell= Cellule(3);
// 			return true;
	}
// 		return false;
}


bool Foret::NextMove()
{
	bool modif= false;
	for (int i= 0; i< lignes; ++i){
// 		for_each<Cellule>(matrice[i].begin(), matrice[i].end(), transition);
	
		for (vector<Cellule>::iterator j(matrice[i].begin()); j!=matrice[i].end(); ++j){
			// 		utilise un booléen pour initialiser
			if (j->getEtat()==2){
// 				if ((j-1)!= matrice[i].begin())
// 					*(j-1)= Cellule(2);
					
				*j= Cellule(3);
				modif= true;
			}
		}
	}
	
	return modif;
}
