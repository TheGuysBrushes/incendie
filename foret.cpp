#include "foret.h"

#include <fstream>
#include <cstdlib>
#include <ctime>

#include <curses.h> // temp
// #include <bits/stl_algo.h>
// #include <bits/algorithmfwd.h>

#define DEBUG_FILE 0
#define DEBUG_ESSENCE 0


// -- A mettre dans le readme ?

// un arbre hérite de cellule
// - les cellules/arbres sont stockés par pointeurs dans le vecteur de vecteurs
// - la cellule ne contient qu'un état, si c'est un arbre (dynamic_cast) il a des attributs spécifiques
// - faire une sous-classe de arbre : arbre en cendres ? ou alors sous-classe de cellule ou seulement arbre dans état brulé (3) : plus simpe

/* TODO	*/
//	DID : 1	: utiliser les coordonnées des arbres dans les méthodes plutôt que de les passer en arguments
// 			+ code nettoyé (commentaires ...)
//	DID	2	: revérifier les arguments des méthodes et les algorithmes pour prendre en compte les modif
//	DID	3	: faire des accesseurs et setters plus propres et explicites, supprimer attributs protected ?
// DID	4	: Prise en compte des PV
// DID	5	: Gérer arbres adjacents diagonaux ET avec plus de 1 de distance
//			5 bis	: reduire la transmission selon la distance ?, creer classe ? (Arbre+distance)
// 		6	: Prise en compte coefs
//			7	: Ajout des vents

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

unsigned Foret::essenceRandom(int _j, int _i, unsigned distOthers){
	int i = _i;
	int j = _j;
	
	// Initialisation du tableau de pondération
	unsigned probaEss[essences.size()];
	for(unsigned k=0;k<essences.size();++k){
		probaEss[k] = 1;
	}
	
	// Récupération des voisins
	list<Arbre*> voisins = adjacents(j, i, distOthers);
	// Pondération du tableau
	for (list< Arbre* >::const_iterator a(voisins.begin()); a!=voisins.end(); ++a){
		probaEss[(*a)->getEssence()->getIndice()] += 3;
	}
	
	unsigned index_max = 0;
	for(unsigned l=0;l<essences.size();++l)
		index_max += probaEss[l];
	
	unsigned index = rand()%index_max;
	unsigned ess = 0;
	while(index >= probaEss[ess] && ess < essences.size()){
		index -= probaEss[ess];
		++ess;
	}
	return ess;
}


// ###################################
//		Initialisations
// ################################### 


/**
 * Charge des essences dans le tableau d'essences à partir d'un fichier texte
 * 	Format des lignes : "Nom" "masse volumique (kg/m³)" "diametre moyen dans R" "hauteur moyenne"  "type (0/1)"
 * @param fileName nom du fichier qui contient les essences
 * @author Ugo
 */
bool Foret::loadEssences(const string& fileName)
{
	// Initialisation du vecteur d'essence
	ifstream f (fileName.c_str());
	#if DEBUG_FILE==1
	cout<< "fichier ouvert?" <<endl;
	#endif
	if(f){
		string line;
		
		#if DEBUG_FILE==1
		cout <<"oui"<< endl;
		#endif
		
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
			delete(tokens);
		}
		
		#if DEBUG_FILE==1
		for(vector<Essence>::const_iterator i(essences.begin()); i!=essences.end(); ++i){
			cout << i->afficheEssence() << endl;
		}
		#endif
		return true;
	}
	else {
		#if DEBUG_FILE==1
		cout << "non" <<endl;
		#endif
		return false;
	}
}

/**
 * initialise une matrice avec des cellules (vierge)
 * @author Ugo and Florian
 */
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


/**
 * Initialise une matrice vide puis ajoute des arbre dans la Foret
 * @param probabilite chance qu'a un arbre d'être placé sur chaque case
 * @author Florian and Ugo
 */
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
		#if DEBUG_ESSENCE==1
		cout << endl<< "indices des essences choisies : ";
		#endif
		
		for (int j= 0; j< colonnes; ++j){
			
			// un nombre est choisi aléatoirement entre 0 et MAXI-1 compris, cela défini notre précision,
			int test= rand()%MAXI;
			// on définit un nombre minimum, le nombre sera supérieur ou inférieur au seuil
			int seuil= MAXI*(1-probabilite);
			
			// si le nombre est supérieur au seuil, c'est un arbre
			if (test>seuil){
				// on choisit une essence aléatoirement pour cet arbre
				unsigned ess = essenceRandom(j,i, 2);
				#if DEBUG_ESSENCE==1
				cout << ess << " ; ";
				#endif
				
				/*
				 * On choisit aléatoirement l'âge et l'humidité de l'arbre qui va être créé
				 * Age [20;100[
				 * Humidité [20;80[ 
				 */
				
				Arbre* ab= new Arbre(j, i, &(essences.at(ess)), rand()%80+20,rand()%60+20 );
				delete(matrice[i][j]); // suppression ancienne cellule
				matrice[i][j]= ab;
			}
		}
	}
}


/**
 * Initialise les différentes essences et la matrice de l'automate
 * @param proba chance qu'un arbre soit placé sur chaque case
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
// 	Modification des éléments
// ###################################
/**
 * Enflamme une cellule si elle existe, selon sa position dans la matrice
 * @author Florian
 * @param row ligne où est la cellule 
 * @param col colonne où est la cellule
 */
void Foret::enflammer(int row, int col)
{
	Cellule* tmp= matrice[row][col];
	
	if (tmp->getEtat()==1){
		// TODO refaire propre (2 dynamic_cast)
		onFire.push_back(dynamic_cast < Arbre* >(tmp));
		dynamic_cast < Arbre* >(tmp)->enflammer();
	}
}

/**
 * Enflamme un arbre
 * @author Florian
 * @param ab arbre à enflammer
 * TODO vérifier qu'il n'est pas déja enflammé (dans onFire) ? : 
 * vérification plus coûteuse que de faire plusieurs calcul de transmission MAIS risque poser soucis de PV
 */
void Foret::enflammer(Arbre* ab)
{
	ab->enflammer();
	onFire.push_back(ab);
}

/**
 * Retourne les arbres qui sont proches d'une cellule donnée
 * @author Ugo Florian
 * @param _col indice de la colonne de la cellule
 * @param _row indice de la ligne de la cellule
 * @param _distance distance sur laquelle s'effectue la recherche de voisins
 */
std::list< Arbre* > Foret::adjacents(int _col, int _row, int _distance) const
{
	int col = _col;
	int row = _row;
	int distance = _distance;
	
	list< Arbre* > liste;
	
	// on prend la taille de la case, sans adjacents supposé
	int larg= 1;	int haut= 1;
	
	// on initialise la premiere case à vérifier à partir des coordonnées de l'arbre
	int posRow= row;	int posCol= col;
	
	// si il y a N cases à gauche, on place la premiere cellule sur la gauche
	if (col>=distance){
		posCol= col-distance;
		larg += distance;
	}
	else  // sinon, on place la premiere au bord de la matrice
	{
		posCol= 0;
		larg += col;
	}
	
	// si il y a N cases à droite, la hauteur du carré est augmenté	de N
	if (col<colonnes-distance)
		larg += distance;
	else // sinon on l'augmente du nombre de cases à droite (nbTotal-pos_cell)
		larg += (colonnes-1)-col;
	
	// si il y a N cases au dessus, on place la premiere cellule au dessus direct
		if (row>=distance) {
			posRow= row-distance;
			haut += distance;
		}
		else // sinon, on place la premiere au bord de la matrice
	{
		posRow= 0;
		haut += row;
	}
	
	// si il y a N cases en dessous, la hauteur du carré est augmenté de N
	if(row <lignes-distance )
		haut += distance;
	else // sinon on l'augmente du nombre de cases en dessous (nbTotal-pos_cell)
		haut += (lignes-1)-row;
	
	int posRowMax= posRow + haut;
	int posColMax= posCol + larg;
	
	// on ajoute les arbres adjacents dans un carré de haut par larg autour de la cellule
	//(on vérifie la cellule elle-même, mais elle vide pour le moment)
	for (int i= posRow; i<posRowMax; ++i){
		
		for (int j= posCol; j<posColMax; ++j){
			
			Cellule* cell= matrice[i][j];
			// verification que la cellule est un arbre, qui n'est pas enflammee
			if (cell->getEtat()==1)
				liste.push_back( dynamic_cast < Arbre* >(cell) );
		}
	}
	return liste;
}


std::list< Arbre* > Foret::adjacents(const Arbre * ab, int _distance) const
{
	return adjacents(ab->getPos().col,ab->getPos().row, _distance);
}

// ###################################
// #		Avancée du temps		  #
// ################################### 

/**
 * Applique une transition de l'état t à l'état t+1 d'un arbre
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

