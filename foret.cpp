#include "foret.h"

#include <fstream>

#include <cstdlib>
#include <ctime>
#include <curses.h> // temp
// #include <bits/stl_algo.h>
// #include <bits/algorithmfwd.h>

// -- A mettre dans le readme ?

// un arbre hérite de cellule
// - les cellules/arbres sont stockés par pointeurs dans le vecteur de vecteurs
// - la cellule ne contient qu'un état, si c'est un arbre (dynamic_cast) il a des attributs spécifiques
// - faire une sous-classe de arbre : arbre en cendres ? ou alors sous-classe de cellule ou seulement arbre dans état brulé (3) : plus simpe

//	DID : 1	: utiliser les coordonnées des arbres dans les méthodes plutôt que de les passer en arguments
// 			+ code nettoyé (commentaires ...)
//	TODO	2	: EN COURS ; revérifier les arguments des méthodes et les algorithmes pour prendre en compte les modif
//			3	: faire des accesseurs et setters plus propres et explicites, supprimer attributs protected ?
// 		4	: Prise en compte des PV
// 		5	: Gérer arbres adjacents diagonaux ET avec plus de 1 de distance, creer classe ? (Arbre+distance)
// 		6	: Prise en compte coefs

using namespace std;

Foret::Foret(int n_lignes, int n_colonnes, float proba)
	: lignes(n_lignes), colonnes(n_colonnes)
{
	initialisation(proba);
}


/**
 * Découpe une chaine de caractères en sous-chaine et place chaque élément dans un vecteur de string
 * @param str : Chaine à découper
 * @author Ugo
 */
vector< string >* Foret::explode(const string& str)
{
	istringstream split(str);
	vector<string> *tokens = new vector<string>;
	for(string each;getline(split, each, ' '); tokens->push_back(each));
		 return   tokens;
}


/**
 * Initialise la matrice
 * @param probabilite chance qu'a un arbre d'etre place sur chaque case
 * @author Ugo and Florian
 */
void Foret::initialisation(float probabilite)
{
	// Initialisation du vecteur d'essence
	
	ifstream f ( "../essence_data.txt");
	cout << "fichier ouvert?" <<endl;
	
	if(f){
		string line;
		
		while(getline(f,line)){
			vector<string>* tokens = explode(line);
			string name = tokens->at(0);
			int masse = atoi(tokens->at(1).c_str());
			float h = strtof(tokens->at(2).c_str(),NULL);
			float d = strtof(tokens->at(3).c_str(),NULL);
			bool t = atoi(tokens->at(4).c_str());
			tabEss.push_back(Essence(name,masse,h,d,t));
			
		}
		
		for(vector<Essence>::const_iterator i(tabEss.begin()); i!=tabEss.end(); ++i){
			cout << i->afficheEssence() << endl;
		}
		
	}
	else cout << "non" <<endl;
	
	
	srand(std::time(0));
	if (probabilite>1){
		probabilite=0.6;
		cout << "MIS A DEFAUT"<< endl;
	}
	
	for (int i= 0; i< lignes; ++i){
		// création d'une nouvelle "ligne de la matrice"
		std::vector< Cellule* > tmp;
		
		for (int j= 0; j< colonnes; ++j){
			
			// un nombre est choisi aléatoirement entre 0 et MAXI-1 compris, cela défini notre précision,
			int test= rand()%MAXI;
			// on défini un nombre minimum, le nombre sera supérieur ou inférieur au seuil
			int seuil= MAXI*(1-probabilite);
			
			// si le nombre est supérieur au seuil, c'est un arbre
			// TODO définir des palier par types d'arbres/essences ... ?
			if (test>seuil){
				Arbre* ab= new Arbre(j, i, &(tabEss[0]), 50, 0.5);
				tmp.push_back(ab);
			}
			else	// sinon c'est une cellule "vide"
				tmp.push_back( new Cellule(0) );
		}
		
		// ajout de la ligne dans la matrice
		matrice.push_back(tmp);
	}
	
// DEPARTS D'INCENDIES
	enflammer(lignes/2, colonnes/2);
	enflammer(lignes/2+1, colonnes/2);
	enflammer(lignes/2, colonnes/2 +1);
	
	enflammer(lignes/2+1, 0);
	enflammer(lignes/2+1, 1);
}


// ###################################
//	Acces aux elements
// ################################### 


// ###################################
// 	Modification des éléments
// ###################################
/**
 * Enflamme une cellule, SI elle existe, selon sa position dans la matrice
 * @author Florian
 * @param row ligne où est la cellule 
 * @param col colonne où est la cellule
 */
void Foret::enflammer(int row, int col)
{
	Cellule* tmp= matrice[row][col];
	
	if (tmp->getEtat()==1){
		// TODO refaire propre
		onFire.push_back(dynamic_cast < Arbre* >(tmp));
		dynamic_cast < Arbre* >(tmp)->enflammer();
	}
}

/**
 * Enflamme un arbre
 * @author Florian
 * @param ab arbre à enflammer
 * TODO vérifier qu'il n'est pas déja enflamme (dans onFire) ? : vérification plus coûteuse que de faire plusieurs calcul de transmission MAIS risque poser soucis de PV
 */
void Foret::enflammer(Arbre* ab)
{
	ab->enflammer();
	onFire.push_back(ab);
}


// void Foret::eteindre(int row, int col)
// {
// 	dynamic_cast < Arbre* >(matrice[row][col])->blast();
// // 	onFire.remove_if<Cellule>();
// }

/**
 * Retourne les arbres qui sont proches d'un arbre donné
 * @author Florian
 * @param ab arbre dont on veut connaître les voisins
 */
std::list< Arbre* > Foret::adjacents(const Arbre* ab) const
{
	int col= ab->getPos().col;
	int row= ab->getPos().row;
	
	list< Arbre* > liste;
	
	if (col<colonnes-1) {
		Cellule* cell= matrice[row][col+1];
		// verification que la cellule est un arbre, qui n'est pas enflammee
		if (cell->getEtat()==1)
			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 		liste.push_back(Coordonnee(x+1, y));
	}
		
	if (col>0) {
		Cellule* cell= matrice[row][col-1];
		// verification que la cellule est un arbre, qui n'est pas enflammee
		if (cell->getEtat()==1)
			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 		liste.push_back(Coordonnee(x-1, y));
	}
	
	if (row<lignes-1) {
		Cellule* cell= matrice[row+1][col];
		// verification que la cellule est un arbre, qui n'est pas enflammee
		if (cell->getEtat()==1)
			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 		liste.push_back(Coordonnee(x, y+1));
	}

	if (row>0) {
		Cellule* cell= matrice[row-1][col];
		// verification que la cellule est un arbre, qui n'est pas enflammee
		if (cell->getEtat()==1)
			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 		liste.push_back(Coordonnee(x, y-1));
	}

	return liste;
}


// ###################################
//	Avancee du temps
// ################################### 

/**
 * applique une transition de l'etat t à l'etat t+1 d'un arbre
 * @author Florian et Ugo
 * @param ab
 */
void Foret::transition(Arbre* ab)
{
// 	printw("Transition %d, %d ; ", x, y); refresh();
		ab->blast();
		
		list< Arbre* > voisins= adjacents(ab);/*adjacents(Coordonnee(row, col));*/
		for (list< Arbre* >::iterator a(voisins.begin()); a!=voisins.end(); ++a){
			enflammer( (*a) );
		}
}

/**
 * passe de t à t+1 tous les arbres à l'aide de la liste d'arbres en feu
 * @author Florian et Ugo
 */
bool Foret::NextMove()
{
	bool modif= false;
	
// 		for_each<Cellule>(matrice[i].begin(), matrice[i].end(), transition);
	
	if (!onFire.empty()){
		modif= true;
		
		list< Arbre* > old= onFire;
		onFire.clear();
		
		// TODO utiliser mapping
		for (list< Arbre* >::iterator ab(old.begin()); ab!=old.end(); ++ab){
			transition(*ab);
		}
	}
	
	return modif;
}

