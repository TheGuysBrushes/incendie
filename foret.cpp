#include "foret.h"

#include <fstream>
#include <cstdlib>
#include <ctime>
// #include <bits/stl_algo.h>
// #include <bits/algorithmfwd.h>

#include <curses.h> // temp



// -- A mettre dans le readme ?

// un arbre hérite de cellule
// - les cellules/arbres sont stockés par pointeurs dans le vecteur de vecteurs
// - la cellule ne contient qu'un état, si c'est un arbre (dynamic_cast) il a des attributs spécifiques
// - faire une sous-classe de arbre : arbre en cendres ? ou alors sous-classe de cellule ou seulement arbre dans état brulé (3) : plus simpe

/* TODO Taches	*/
//	DID : 1	: utiliser les coordonnées des arbres dans les méthodes plutôt que de les passer en arguments
// 			+ code nettoyé (commentaires ...)
//	DID	2	: revérifier les arguments des méthodes et les algorithmes pour prendre en compte les modif
//	DID	3	: faire des accesseurs et setters plus propres et explicites, supprimer attributs protected ?
// DID	4	: Prise en compte des PV
// DID	5	: Gérer arbres adjacents diagonaux ET avec plus de 1 de distance
//			5 bis	: reduire la transmission selon la distance ?, creer classe ? (Arbre+distance)
// 		6	: EN COURS	; Prise en compte coefs
//			7	: Ajout des vents

//			8	: EN COURS	; Implementation de Qt
// 		9	: Debut d'interface de configuration (taille, vitesse ...)

//			10	: Jeux d'essais

using namespace std;


// ###################################
//		Constructeurs et destructeur
// ################################### 

Foret::Foret(int n_lignes, int n_colonnes, float proba, float coefFire)
: lignes(n_lignes), colonnes(n_colonnes), burningCoef(coefFire), wind(0.0,0.0)
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
 * @author Ugo
 * @param str : Chaine à découper
 * @return vecteur des mots séparés par des espaces // TODO TRIM ? + remplacer "_" par des espaces
 */
// TODO voir si il faut utiliser référence ou pointeur
vector< string >& explode(const string& str)
{
	istringstream split(str);
	vector< string >* tokens = new vector<string>;
	
	for(string each; getline(split, each, ' '); tokens->push_back(each.c_str()) );
	
	#if DEBUG_FILE
	cout << "vecteur : \t";
	for (vector<string>::const_iterator i(tokens->begin()); i!= tokens->end(); ++i){
		cout << *i<< "|";
	}
	cout << endl;
	#endif

	return   *tokens;
}

/**
 * Choisi une essence pseudo aléatoirement, en prennant en compte les arbre autour d'une position
 * @author Ugo
 * @param j colonne de la position du futur arbre
 * @param i ligne de la position du futur arbre
 * @param distOthers distance à laquelle on doit prendre en compte les arbres avoisinants
 * @return indice, dans le tableau d'essence, de l'essence "tirée"
 */
unsigned Foret::essenceRandom(int j, int i, unsigned distOthers){

	// Initialisation du tableau de pondération
	unsigned probaEss[essences.size()];
	for(unsigned k=0; k<essences.size(); ++k){
		probaEss[k] = 1;
	}
	
	// Récupération des voisins
	list<Arbre*> voisins = adjacents(j, i, distOthers);
	// Pondération du tableau
	unsigned densite= 5;	// plus le parametre est élevé, plus les chances que les arbres soit les mêmes que leurs voisins est forte (formation bosquets)
	for (list< Arbre* >::const_iterator a(voisins.begin()); a!=voisins.end(); ++a){
		probaEss[(*a)->getEssence()->getIndice()] += densite;
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
 * @author Ugo
 * @param fileName nom du fichier qui contient les essences
 * @return vrai si le fichier a été ouvert, faux sinon
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
			#if DEBUG_FILE
			cout << endl<< "ligne : \t"<< line<< endl;
			#endif
			
			vector<string>& tokens = explode(line);	
			// conversion du diametre en float
			float x; istringstream(tokens[2])>> x;
			essences.push_back( Essence(indice,
												tokens[0], atoi(tokens[1].c_str()),
												x,
												atoi(tokens[3].c_str()),
												atoi(tokens[4].c_str()),
												atoi(tokens[5].c_str())			) );
			#if DEBUG_FILE
			cout << "atof : diam : " << x<< endl;
			#endif
			
			indice +=1;
// 			delete(tokens);
		}
		
		#if DEBUG_FILE==1
		showEssences();
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
 * @author Florian and Ugo
 * @param probabilite chance qu'a un arbre d'être placé sur chaque case
 */
void Foret::randomMatrice(float probabilite)
{
	srand(std::time(0));
	if (probabilite>1){
		probabilite=0.6;
		std::cout << "MIS A DEFAUT"<< endl;
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
				unsigned distVoisins= 2;
				unsigned ess = essenceRandom(j,i, distVoisins);
				#if DEBUG_ESSENCE==1
				cout << ess << " ; ";
				#endif
				
				/*
				 * On choisit aléatoirement l'âge et l'humidité de l'arbre qui va être créé
				 * Age [20;100[
				 * Humidité [20;70[ 
				 */
				
				Arbre* ab= new Arbre(matrice[i][j], j, i, &(essences[ess]), rand()%80+20,rand()%70+20 );
// 				delete(matrice[i][j]); // suppression ancienne cellule TODO delete this comment
				matrice[i][j]= ab;	// TODO verifier si on peut supprimer cette ligne en faisant l'operation dans le constructeur de Arbre
			}
		}
	}
		
	#if DEBUG_ESSENCE==1
	cout << "max : "<< endl;  
	Arbre dummy1(-1, -1, &(essences[3]), 99, 69 );
	cout << "min : "<< endl;  
	Arbre dummy2(-1, -1, &(essences[2]), 20, 20 );
	#endif
}


/**
 * Initialise les différentes essences et la matrice de l'automate
 * @author Ugo and Florian
 * @param proba chance qu'un arbre soit placé sur chaque case
 */
void Foret::initialisation(float proba)
{
	loadEssences("../essence_data.txt");
	randomMatrice(proba);
	
	// DEPARTS D'INCENDIES
	allumer(lignes/2, colonnes/2);
	allumer(lignes/2+1, colonnes/2);
	allumer(lignes/2, colonnes/2 +1);
	
	allumer(lignes/2+1, 0);
	allumer(lignes/2+1, 1);
}

// ###################################
// 	Modification des éléments
// ###################################

void Foret::allumer(int row, int col)
{
	Cellule* tmp= matrice[row][col];
	
	int etat= tmp->getEtat();
	
	if (etat==1){
		Arbre * ab = dynamic_cast<Arbre *>(tmp);
		allumer(ab);
	}
}

void Foret::allumer(Arbre* ab)
{
	ab->burn();
	onFire.push_back(ab);
}


/**
 * Enflamme une cellule si elle existe, selon sa position dans la matrice
 * @author Florian
 * @param row ligne où est la cellule 
 * @param col colonne où est la cellule
 * @deprecated
 */
void Foret::enflammer(int row, int col)
{
	Cellule* tmp= matrice[row][col];
	
	int etat= tmp->getEtat();
	
	if (etat==1){
		Arbre * ab = dynamic_cast<Arbre *>(tmp);
		enflammer(ab);
	}
// 	else if (etat==2){}
}

/**
 * Enflamme un arbre
 * @author Florian
 * @param ab arbre à enflammer
 */
void Foret::enflammer(Arbre* ab)
{
// 	if (ab->getEtat==2){
// // 		ab->brule(); TODO verifier si lorsque l'arbre est ajouté plusieurs fois dans la liste des adjacents, il commence à brûler, brule plus fort/vite ?
// 	}
// 	else {
	ab->enflammer(burningCoef);
	if (ab->isOnFire())
		onFire.push_back(ab);
// 	}
}

/**
 * Retourne les arbres qui sont proches d'une cellule donnée
 * @author Florian and Ugo
 * @param col indice de la colonne de la cellule
 * @param row indice de la ligne de la cellule
 * @param distance distance sur laquelle s'effectue la recherche de voisins
 * @return list de pointeurs sur arbres // TODO ajout attribut distance ?
 */
std::list< Arbre* > Foret::adjacents(int col, int row, int distance) const
{
//	Voir les images pour modif ? calcul transmission selon distance (1.4/distance) -0.4 ? : transmission proportionnelle inverse à distance de 0 à 3.33
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

/**
 * Retourne les arbres qui sont proches d'une cellule donnée, appelle adjacents(int, int)
 * @author Florian
 * @param ab arbre dont on veut connaître les voisins
 * @param distance distance sur laquelle s'effectue la recherche de voisins
 * @return list de pointeurs sur arbres
 */
std::list< Arbre* > Foret::adjacents(const Arbre * ab, int distance) const
{
	return adjacents(ab->getPos().col,ab->getPos().row, distance);
}

// #############################
// #		Avancée du temps		  #
// ############################# 
/**
 * Applique une transition de l'état t à l'état t+1 d'un arbre
 * @author Florian et Ugo
 * @param ab
 */
void Foret::transition(Arbre* ab)
{
	unsigned distAdj= 1; // distance à laquelle les voisins seront enflammes ; TODO dist 1= contact, dist 2= diagonaux, dist 3 =
	list< Arbre* > voisins= adjacents(ab, distAdj);
	for (list< Arbre* >::iterator a(voisins.begin()); a!=voisins.end(); ++a){
		enflammer( (*a) );
	}
	
	if ( ab->brule(burningCoef) )
		onFire.push_back(ab);
}

/**
 * passe de t à t+1 tous les arbres à l'aide de la liste d'arbres en feu
 * @author Florian et Ugo
 * @return vrai si il y eu des changements, faux sinon
 */
bool Foret::NextMove()
{
	bool modif= false;
	
	if (!onFire.empty()){
		modif= true;		
		list< Arbre* > old= onFire;
		onFire.clear();		
		// TODO utiliser mapping ?
		for (list< Arbre* >::iterator ab(old.begin()); ab!=old.end(); ++ab){
			transition(*ab);
		}
	}	
	return modif;
}

// #########################
// #	Affichage attributs	#
// ######################### 
void Foret::showEssences() const
{
	cout << "| Nom\t\t||"<< " indice\t||"<< " masseV\t||"<< " diam\t||"<< " haut\t||"<< " type\t||"<< " humidité\t|"<< endl;
	cout << "-------------------------------------------------------------------------------"<< endl;
	
	for(vector<Essence>::const_iterator i(essences.begin()); i!=essences.end(); ++i){
		string nom= i->getName();
		if (nom.size()<=9)
			nom+="\t";
		
		cout << "| "<< nom<< "\t| "<< 
		i->getIndice()		<< "\t| "<<
		i->getMasse()		<< "\t| "<<
		i->getDiametre()	<< "\t| "<< 
		i->getHauteur()		<< "\t| "<<
		i->getType() 		<< "\t|"<<
		" ?? \t|"<< endl;
	}
}
