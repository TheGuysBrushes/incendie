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
//	TODO	2	: EN COURS	; revérifier les arguments des méthodes et les algorithmes pour prendre en compte les modif
//			3	: faire des accesseurs et setters plus propres et explicites, supprimer attributs protected ?
// 		4	: EN COURS	; Prise en compte des PV
// 		5	: EN PARTIE	; Gérer arbres adjacents diagonaux ET avec plus de 1 de distance, creer classe ? (Arbre+distance)
// 		6	: Prise en compte coefs

using namespace std;


// ###################################
//		Constructeurs et destructeur
// ################################### 

Foret::Foret(int n_lignes, int n_colonnes, float proba)
	: lignes(n_lignes), colonnes(n_colonnes)
{
	initialisation(proba);
}

Foret::~Foret()
{
	for (int i= 0; i< lignes; ++i){
		for (vector< Cellule* >::iterator j( matrice[i].begin() ); j!=matrice[i].end(); ++j){
			delete *j;
		}
	}
	
	onFire.clear();
}


// ###################################
//		Fonctions Utiles
// ################################### 

/**
 * Découpe une chaine de caractères en sous-chaine et place chaque élément dans un vecteur de string
 * 	nécessaire pour l'initialisation des essences d'arbres
 * @param str : Chaine à découper
 * @author Ugo
 */
vector< string >* explode(const string& str)
{
	istringstream split(str);
	vector<string> *tokens = new vector<string>;
	for(string each;getline(split, each, ' '); tokens->push_back(each));
		 return   tokens;
}


// ###################################
//		Initialisations
// ################################### 

bool Foret::loadEssences(const string& fileName)
{
	// Initialisation du vecteur d'essence
	ifstream f (fileName.c_str());
	cout << "fichier ouvert?" <<endl;

	if(f){
		string line;
		cout <<"oui"<< endl;
		int indice = 0;
		while(getline(f,line)){
			vector<string>* tokens = explode(line);
			string name = tokens->at(0);
			int masse = atoi(tokens->at(1).c_str());
			float h = strtof(tokens->at(2).c_str(),NULL);
			float d = strtof(tokens->at(3).c_str(),NULL);
			bool t = atoi(tokens->at(4).c_str());
			essences.push_back(Essence(indice,name,masse,h,d,t));
			indice +=1;
		}
		
		for(vector<Essence>::const_iterator i(essences.begin()); i!=essences.end(); ++i){
			cout << i->afficheEssence() << endl;
		}
		
		return true;
	}
	else {
		cout << "non" <<endl;
		return false;
	}
}				
void Foret::initEmpty()
{
		for (int i= 0; i< lignes; ++i){
			// création d'une nouvelle "ligne de la matrice"
			std::vector< Cellule* > tmp;
		
			for (int j= 0; j< colonnes; ++j){
				tmp.push_back( new Cellule(0) );
			}
		
			// ajout de la ligne dans la matrice
			matrice.push_back(tmp);
		}
}

void Foret::randomMatrice(float probabilite)
{
	srand(std::time(0));
	if (probabilite>1){
		probabilite=0.6;
		cout << "MIS A DEFAUT"<< endl;
	}
	
	// Initialisation de l'ensemble des cellules à vide
	initEmpty();
	
	for (int i= 0; i< lignes; ++i){
		// création d'une nouvelle "ligne de la matrice"
// 		std::vector< Cellule* > tmp= *matrice[i];
		
		for (int j= 0; j< colonnes; ++j){
			
			// un nombre est choisi aléatoirement entre 0 et MAXI-1 compris, cela défini notre précision,
			int test= rand()%MAXI;
			// on défini un nombre minimum, le nombre sera supérieur ou inférieur au seuil
			int seuil= MAXI*(1-probabilite);
			
			// si le nombre est supérieur au seuil, c'est un arbre
			if (test>seuil){
				// TODO Gérer l'apparition de bosquet, i.e, probabilité augmentée qu'un arbre soit de la même essence que ses voisins
				
				// Initialisation du tableau de pondération
				unsigned int probaEss[essences.size()];
				for(unsigned int k=0;k<essences.size();++k){
					probaEss[k] = 1;
				}
				// Récupération des voisins
				list<Arbre*> voisins = adjacents(j,i,2);
				// Pondération du tableau
				for (list< Arbre* >::iterator a(voisins.begin()); a!=voisins.end(); ++a){
					probaEss[(*a)->getEssence()->getIndice()] += 4;
				}
				
				unsigned int index_max = 0;
				for(unsigned int l=0;l<essences.size();++l)
					index_max += probaEss[l];
				
				unsigned int index = rand()%index_max;
				unsigned int ess = 0;
				while(index > probaEss[ess] && ess < essences.size()){
					index -= probaEss[ess];
					++ess;
				}
				cout << "indice de l'essence choisie : " << ess << endl;
				// Sélectionner une essence dans le tableau de proba pondéré
				//int ess = rand()%2;
								
				// Constructeur d'arbre a été modifié mais ça ne change pas la signature de la création ci-dessous
				Arbre* ab= new Arbre(j, i, &(essences.at(ess)), 20, 10);
// 				tmp.push_back(ab);
				delete(matrice[i][j]);
				matrice[i][j]= ab;
			}
// 			else	// sinon c'est une cellule "vide"
// 				tmp.push_back( new Cellule(0) );
		}
		
		// ajout de la ligne dans la matrice
// 		matrice.push_back(tmp);
	}
}


/**
 * Initialise les différentes essences et la matrice de l'automate
 * @param probabilite chance qu'a un arbre d'etre place sur chaque case
 * @author Ugo and Florian
 */
void Foret::initialisation(float proba)
{
	loadEssences("../essence_data.txt");
	randomMatrice(proba);
	
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
std::list< Arbre* > Foret::adjacents(int _col, int _row, int _distance) const
{
	int col = _col;
	int row = _row;
	int distance = _distance;
	
	list< Arbre* > liste;
	
// On cherche à vérifier les cases autour de la cellule dans un carré de taille donné (ici 3x3)
	/// (Si on veut vérifier dans un carré de taille 5, alors vérifier avec (col>=2) / (row>=2) et (col< colonnes-2) / (row< ligne-2))
	
	// on prend la taille de la case, sans adjacents supposé
	int larg= 1;	int haut= 1;
	
	// on initialise la premiere case à vérifier à partir des coordonnées de l'arbre
	int posRow= row;	int posCol= col;
	
	// si il y a des cases à gauche, on place la premiere cellule sur la gauche directe
	if (col>=distance){
		posCol= col-distance;
		++larg;
	}
	// si il des cases à droite, la hauteur du carré est augmenté	
		/// (si il y a des cases à gauche et à droite, la hauteur est 3)
	if (col<colonnes-distance)
		++larg;
	
	// si il y a des cases au dessus, on place la premiere cellule au dessus direct
	if (row>=distance) {
		posRow= row-distance;
		++haut;
	}
	// si il des cases en dessous, la hauteur du carré est augmenté
		/// (si il y a des cases au dessus et en dessous, il y a 3 cellules de largeur)
	if(row <lignes-distance )
		++haut;

	int posRowMax= posRow + haut;
	int posColMax= posCol + larg;
	
	// on ajoute dans les arbres adjacents dans un carré de haut par larg autour de "ab"
			//(ont vérifie "ab" lui-même, mais il est en feu... donc non ajouté dans les adjacents)
	for (int i= posRow; i<posRowMax; ++i){
		for (int j= posCol; j<posColMax; ++j){
			
			Cellule* cell= matrice[i][j];
			// verification que la cellule est un arbre, qui n'est pas enflammee
			if (cell->getEtat()==1)
				liste.push_back( dynamic_cast < Arbre* >(cell) );
			
		}
	}
	
// 	if (col<colonnes-1) {
// 		Cellule* cell= matrice[row][col+1];
// 		// verification que la cellule est un arbre, qui n'est pas enflammee
// 		if (cell->getEtat()==1)
// 			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 	}
// 		
// 	if (col>0) {
// 		Cellule* cell= matrice[row][col-1];
// 		// verification que la cellule est un arbre, qui n'est pas enflammee
// 		if (cell->getEtat()==1)
// 			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 	}
// 	
// 	if (row<lignes-1) {
// 		Cellule* cell= matrice[row+1][col];
// 		// verification que la cellule est un arbre, qui n'est pas enflammee
// 		if (cell->getEtat()==1)
// 			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 	}
// 
// 	if (row>0) {
// 		Cellule* cell= matrice[row-1][col];
// 		// verification que la cellule est un arbre, qui n'est pas enflammee
// 		if (cell->getEtat()==1)
// 			liste.push_back( dynamic_cast < Arbre* >(cell) );
// 	}

	return liste;
}


std::list< Arbre* > Foret::adjacents(const Arbre * ab, int _distance) const
{
	
	int col= ab->getPos().col;
	int row= ab->getPos().row;
	return adjacents(col, row, _distance);
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
	list< Arbre* > voisins= adjacents(ab,1);
	for (list< Arbre* >::iterator a(voisins.begin()); a!=voisins.end(); ++a){
		enflammer( (*a) );
	}
	
	if ( ab->brule() )
		onFire.push_back(ab);

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

