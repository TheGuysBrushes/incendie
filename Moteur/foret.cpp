#include "foret.h"

#include <cstdlib>
#include <ctime>

int hMin= 20;
int ecartAgeMax= 80;
int ecartHMax= 50;

// -- A mettre dans le readme ?

// un arbre hérite de cellule
// - les cellules/arbres sont stockés par pointeurs dans le vecteur de vecteurs
// - la cellule ne contient qu'un état, si c'est un arbre (dynamic_cast) il a des attributs spécifiques
// - faire une sous-classe de arbre : arbre en cendres ? ou alors sous-classe de cellule ou seulement arbre dans état brulé (3) : plus simpe

/* foret	*/
//	DID : 1	: utiliser les coordonnées des arbres dans les méthodes plutôt que de les passer en arguments
// 			+ code nettoyé (commentaires ...)
//	DID	2	: revérifier les arguments des méthodes et les algorithmes pour prendre en compte les modif
//	DID	3	: faire des accesseurs et setters plus propres et explicites, supprimer attributs protected ?
// DID	4	: Prise en compte des PV
// DID	5	: Gérer arbres adjacents diagonaux ET avec plus de 1 de distance
//			5 bis	: reduire la transmission selon la distance ?, creer classe ? (Arbre+distance)
// TODO	6	: EN PARTIE	; Prise en compte coefs
//	TODO	7	: EN PARTIE	; Ajout des vents

//	DID	8	: Implementation de Qt
// DID	9	: Debut d'interface de configuration (taille, vitesse ...)

//	TODO	10	: Jeux d'essais

//  IMPROVEIT si plusieurs arbres se touchent, le feu est plus fort?

using namespace std;


// ###################################
//		Constructeurs et destructeur
// ################################### 

Foret::Foret(int n_colonnes, int n_lignes, float proba, float coefFire)
: lignes(n_lignes), colonnes(n_colonnes), burningCoef(coefFire)
{
	initialisation(proba);
// TODO Ugo : faire des constructeur qui permettent de créer un vent inital en accord avec la valeur initiale du curseur
	wind = new Vent(2.0,2.0);
}

// Foret::Foret(Foret& other, float proba)
//  : lignes(other.lignes), colonnes(other.colonnes), burningCoef(other.burningCoef)
// {
// 	initialisation(proba);
// }

Foret::Foret(string& filename) :burningCoef(0.5)
{
	load("foret1");
// 	randomMatrix(0.60);
	
	wind = new Vent(2.0,2.0);
}



/**
 * On vide également les listes, mêmes si c'est fait automatiquement TODO verifier si ca n'empeche pas un probleme de "double libération" des arbres  et l'ordre libérer-vider
 */
Foret::~Foret()
{
	for (int i= 0; i< lignes; ++i){
		for (vector< Cellule* >::iterator j( matrix[i].begin() ); j!=matrix[i].end(); ++j){
			delete *j;
		}
	}
	
	onFire.clear();
	burned.clear();
	carbonized.clear();
	uprooted.clear();
	extinguished.clear();
}


// ###################################
//		Fonctions Utiles
// ################################### 

/**
 * Découpe une chaine de caractères en sous-chaine et place chaque élément dans un vecteur de string
 * 	nécessaire pour l'initialisation des essences d'arbres
 * @author Ugo
 * @param str : Chaine à découper
 * @return vecteur des mots séparés par des espaces // TODO TRIM
 */
// IMPROVEIT voir si il faut utiliser référence ou pointeur
vector< string >& explode(const string& str)
{
	istringstream split(str);
	vector< string >* tokens = new vector<string>;
	
	for(string each; getline(split, each, ' '); tokens->push_back( each.c_str()) );
	
	#if DEBUG_FILE
	cout << "vecteur : \t";
	for (vector<string>::const_iterator i(tokens->begin()); i!= tokens->end(); ++i){
		cout << *i<< "|";
	}
	cout << endl;
	#endif

	return *tokens;
}


// ########################
//		Initialisations
// ########################

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
	#if DEBUG_FILE
	cout<< "fichier ouvert?" <<endl;
	#endif
	if(f){
		#if DEBUG_FILE
		cout <<"oui"<< endl;
		#endif
		
		string line;
		
		int indice = 0;
		while(getline(f,line)){
			#if DEBUG_FILE
			cout << endl<< "ligne : \t"<< line<< endl;
			#endif
			
			vector<string>& tokens = explode(line);	
			
			// remplacement des underscores par des espaces
			if ( tokens[0].find('_')!= string::npos ){
				int ind= tokens[0].find('_');
				#if DEBUG_FILE
				cout << "ind de _ : "<< ind<< endl;
				#endif
				
				tokens[0][ind]= ' ';
			}
			
			// conversion du diametre en float
			float x; istringstream(tokens[2])>> x;
			essences.push_back( Essence(indice, tokens[0],
											atoi(tokens[1].c_str()),
											x,
											atoi(tokens[3].c_str()),
											atoi(tokens[4].c_str()),
											atoi(tokens[5].c_str())			) );
			
			indice +=1;
// 			delete(tokens);
		}
		#if DEBUG_FILE
		showEssences();
		#endif
		
		return true;
	}
	else {
		#if DEBUG_FILE
		cout << "non" <<endl;
		#endif
		return false;
	}
}

/**
 * Choisi une essence pseudo aléatoirement, en prennant en compte les arbre autour d'une position
 * @author Ugo
 * @param j colonne de la position du futur arbre
 * @param i ligne de la position du futur arbre
 * @param distOthers distance à laquelle on doit prendre en compte les arbres avoisinants
 * @return indice, dans le tableau d'essence, de l'essence "tirée"
 */
unsigned Foret::essenceRandom(int col, int row, unsigned distOthers){
	
	// Initialisation du tableau de pondération
	unsigned probaEss[essences.size()];
	for(unsigned k=0; k<essences.size(); ++k){
		probaEss[k] = 1;
	}
	
	// Récupération des voisins
	list<Arbre*> voisins = adjacents(col, row, distOthers);
	// Pondération du tableau
	unsigned densite= 5;	// plus le parametre est élevé, plus les chances que les arbres soit les mêmes que leurs voisins est forte (formation bosquets)
	for (list< Arbre* >::const_iterator a(voisins.begin()); a!=voisins.end(); ++a){
		probaEss[(*a)->getEssence()->getIndice()] += densite;
	}
	
	unsigned index_max = 0;
	for(unsigned l=0; l<essences.size(); ++l)
		index_max += probaEss[l];
	
	unsigned index = rand()%index_max;
	unsigned ess = 0;
	while(index >= probaEss[ess] && ess < essences.size()){
		index -= probaEss[ess];
		++ess;
	}
	
	return ess;
}

/**
 *	Inutilisé, car cela ne permet pas de vider les listes, TODO faire une fonction qui permet de les vider
 */
std::list< list< Arbre* > >* Foret::getChanged()
{
	list< list< Arbre* > >* listes= new list< list< Arbre* > >();
	listes->push_back(uprooted);
	listes->push_back(extinguished);
	listes->push_back(burned);
	listes->push_back(carbonized);
	return listes;
}


/**
 * "Plante" un arbre à la position donnée dans la matrice
 * @author Florian et Ugo
 */
void Foret::plantTree(int col, int row)
{
	// on choisit une essence aléatoirement pour cet arbre
	unsigned distVoisins= 2;
	unsigned ess = essenceRandom(col,row, distVoisins);
	#if DEBUG_ESSENCE==1
	cout << ess << " ; ";
	#endif
	
	/*
	 * On choisit aléatoirement l'âge et l'humidité de l'arbre qui va être créé
	 * Age [maturite;100[
	 * Humidité [20;70[ 
	 */
	
	const Essence* pEss= &(essences[ess]);
	// pour l'instant, on considere que tous les arbres ont atteint leur maturite
	Arbre* ab= new Arbre(matrix[row][col], col,row, pEss, rand()%ecartAgeMax +pEss->getAgeMaturite(), rand()%ecartHMax +hMin );
	matrix[row][col]= ab;
}

void Foret::plantTree(int col, int row, unsigned int numEss)
{
	const Essence* pEss= &(essences[numEss]);
	// pour l'instant, on considere que tous les arbres ont atteint leur maturite
	Arbre* ab= new Arbre(matrix[row][col], col,row, pEss, rand()%ecartAgeMax +pEss->getAgeMaturite(), rand()%ecartHMax +hMin );
	matrix[row][col]= ab;
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
		matrix.push_back(tmp);
	}
}


/**
 * Initialise une matrice vide puis ajoute des arbre dans la Foret
 * @author Florian and Ugo
 * @param probabilite chance qu'a un arbre d'être placé sur chaque case
 */
void Foret::randomMatrix(float probabilite)
{
	srand(std::time(0));
	if (probabilite>1){
		probabilite=0.6;
		std::cout << "MIS A DEFAUT"<< endl;
	}
	
	// Initialisation de l'ensemble des cellules à vide
	initEmpty();
	
	// Arbres placés aléatoirement
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
				plantTree(j ,i);
			}
		}
	}
		
	#if DEBUG_PV
	const Essence* pEssMax= &(essences[4]);
	const Essence* pEssMin= &(essences[2]);
	
	cout << endl<<  "====================================="<< endl;
	int age= ecartAgeMax +pEssMax->getAgeMaturite();
	
	cout << "MAXIMUM : "<< endl;
	cout << "\t Age : "<< age<< ", Humidité : "<< hMin +ecartHMax<< ", ";;
	Arbre dummy1(-1, -1, pEssMax, age, ecartHMax +hMin );
	
	age= pEssMin->getAgeMaturite();
	
	cout << "MINUMUM : "<< endl;
	cout << "\t Age : "<< age<< ", Humidité : "<< hMin<< ", ";
	Arbre dummy2(-1, -1, pEssMin, pEssMin->getAgeMaturite(), hMin );
	#endif
}


/**
 * Initialise les différentes essences et la matrice de l'automate
 * @author Ugo and Florian
 * @param proba chance qu'un arbre soit placé sur chaque case
 */
void Foret::initialisation(float proba)
{
	loadEssences("../Moteur/essence_data.txt");
	randomMatrix(proba);
}

/**
 * Met la foret à l'état vierge : aucun arbre
 * @author Florian
 */
void Foret::clean()
{
	onFire.clear();
	carbonized.clear();
	
	// 	suppression de la matrice
	for (int i= 0; i< lignes; ++i){
		for (vector< Cellule* >::iterator j( matrix[i].begin() ); j!=matrix[i].end(); ++j){
			delete *j;
			// suppression d'une "ligne de la matrice"
		}
		matrix[i].clear();
	}
	matrix.clear();
}

/**
 * Définit/redéfinit la taille et le coefficient de brulure d'une foret
 * @author Florian
 */
void Foret::setValues(int largeur, int hauteur, float coef)
{
	lignes = hauteur;
	colonnes = largeur;
	burningCoef = coef;
}

/**
 * Définit la direction du vent
 * @author Florian
 * @param EO force du vent sur l'axe Est-Ouest
 * @param NS force du vent sur l'axe Nord-Sud
 */
void Foret::setWind(int EO, int NS)
{
	wind->setPower_h(EO);
	wind->setPower_v(NS);
}


// ##################################
// 	Modification des éléments
// ##################################

// IMPROVEIT couper un arbre : supprimer arbre puis créer cellule ?, laisser l'arbre ?
/**
 * Coupe ou déracine un arbre (IMPROVEIT pour l'instant l'arbre est laissé)
 * @author Florian
 * @param ab arbre à supprimer
 */
void Foret::uproot(Arbre* ab)
{
	ab->uproot();
	onFire.remove(ab);
	uprooted.push_back(ab);
}

/**
 * Coupe ou déracine l'arbre à la position donnée
 * @author Florian
 * @param ab arbre à supprimer
 */
void Foret::uproot(int col, int row)
{
	Cellule* tmp= matrix[row][col];
	
	if (tmp->getState()>0){	// si il s'agit un arbre
		Arbre * ab = dynamic_cast<Arbre *>(tmp);
		uproot(ab);
	}
}

void Foret::cut(int xDep, int yDep, int xArr, int yArr)
{
	for(int i= xDep; i < xArr; ++i){
		
		for(int j= yDep; j < yArr; ++j){
			matrix[j][i]->setState(-2);
		}
	}
}

void Foret::delay(int xDep, int yDep, int xArr, int yArr)
{
	for(int i= xDep; i < xArr; ++i){
		
		for(int j= yDep; j < yArr; ++j){
			// Le retardateur ne s'applique que sur les arbres
			if(matrix[j][i]->getState() > 0){
				Arbre* ab = dynamic_cast<Arbre *>(matrix[j][i]);
				// Réduit le coefficient de combustion personnel de l'arbre à 0.5
				ab->setCoefficient(0.5);
			}			
		}
	}
}


/**
 * Eteint un arbre en conservant ses points de vie courant
 * @author Florian
 * @param ab arbre à éteindre
 */
void Foret::water(Arbre* ab)
{
// 	for (list< Arbre* >::iterator tmp(onFire.begin()); tmp!=onFire.end(); ++tmp)
// 		if (ab==*tmp)

	ab->water();
	onFire.remove(ab);
	extinguished.push_back(ab);
}

// EMBRASEMENT
/**
 * Definit un arbre comme étant en feu
 * @author Florian
 * @param ab arbre à allumer
 */
void Foret::kindle(Arbre* ab)
{
	ab->kindle();
	onFire.push_back(ab);
	burned.push_back(ab);
}

/**
 * Definit une position comme étant en feu, si c'est un arbre
 * @author Florian
 * @param col_row position de l'arbre à allumer
 */
void Foret::kindle(int col, int row)
{
	Cellule* tmp= matrix[row][col];
	
	int etat= tmp->getState();
	
	if (etat==1){
		Arbre * ab = dynamic_cast<Arbre *>(tmp);
		kindle(ab);
	}
}

// EXTINCTION
/**
 * Definit un arbre comme étant en cendres
 * @author Florian
 * @param ab arbre à mettre en cendres
 */
void Foret::blast(Arbre* ab)
{
	ab->blast();
	carbonized.push_back(ab);
}


// BURNING
/**
 * Enflamme un arbre : un autre arbre lui "transmet" du feu
 * @author Florian
 * @param ab arbre à enflammer
 */
void Foret::spark(Arbre* ab, int intensite)
{
// 	if (ab->getState==2){
// // 		ab->brule();
// 	}
// 	else {
	ab->spark(burningCoef*intensite);
	if (ab->isOnFire()){
		onFire.push_back(ab);
		burned.push_back(ab);
	}
}

/**
 * Enflamme une cellule si elle existe, selon sa position dans la matrice
 * @author Florian
 * @param row ligne où est la cellule 
 * @param col colonne où est la cellule
 * @deprecated
 */
void Foret::spark(int col, int row, int intensite)
{
	Cellule* tmp= matrix[row][col];
	
	int etat= tmp->getState();
	
	if (etat==1){
		Arbre * ab = dynamic_cast<Arbre *>(tmp);
		spark(ab, intensite);
	}
// 	else if (etat==2){}
}


/**
 * Retourne les arbres qui sont proches d'une cellule donnée
 * @author Florian and Ugo
 * @param col indice de la colonne de la cellule
 * @param row indice de la ligne de la cellule
 * @param distance distance sur laquelle s'effectue la recherche de voisins
 * @return list de pointeurs sur arbres // IMPROVEIT ajout attribut distance ?
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
			
			Cellule* cell= matrix[i][j];
			// verification que la cellule est un arbre, qui n'est pas enflammee
			if (cell->getState()==1)
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

void Foret::burnAdjacentsWind(int posCol, int posRow, int hor, int vert){
	for(int i = 0; i < abs( hor ); ++i) {
		for(int j = 0; j < abs( vert ); ++j) {
// 			burnAdjacentsWind(posCol, posRow, i, j);

			if( ( (hor + posCol-i) >= 0 ) && ( (hor + posCol-i) < (colonnes) ) && ( (vert + posRow-j) >= 0 ) && ( (vert +posRow-j) < (lignes) ) ){
				
				#if DEBUG_VENT
				cout << "transmission à cellule en "<< posRow + (hor -i)<< " ; "<< posCol + (vert -j)<< endl;
				#endif
				Cellule* cell = matrix[posRow +  (vert -j)][posCol + (hor -i)];
				
				if(cell->getState() == 1)
					spark(dynamic_cast < Arbre* >(cell), (i+1)*(j+1) );
			}
		}
	}
}

void Foret::burnAdjacentsWind(Arbre* a, const Vent* vent)
{
#if DEBUG_VENT
cout<< endl<< "BRULE LES ADJACENTS DE "<< a->getPos().col<< ";" << a->getPos().row<< endl;
#endif

	burnAdjacentsWind(a->getPos().col, a->getPos().row,
						vent->getPower_h(), vent->getPower_v() );
	
	// TODO renommer les variables h et l avec de vrais noms
// 	int h = wind->getPower_h();
// 	int l = wind->getPower_v();
	
// 	for(int i = 0; i <= abs( h ); ++i) {
// 		for(int j = 0; j <= abs( l ); ++j) {
// 			// On vérifie que la cellule spécifiée est dans la matrice IMPROVEIT ce n'est pas performant
// 			if( ( (hor + row-i) >= 0 ) && ( (hor + row-i) < (lignes) ) && ( (vert + col-j) >= 0 ) && ( (vert +col-j) < (colonnes) ) ){
// 				FIXIT hor/i et vert/j inverses ?
// 				Cellule* cell = matrix[row + (hor -i)][col + (vert -j)];
// 				
// 				if(cell->getState() == 1)
// 					spark(dynamic_cast < Arbre* >(cell));
// 			}
// 			
// 		}
// 	}
	
}


// #############################
// #		Avancée du temps		  #
// ############################# 
/**
 * Applique une transition de l'état t à l'état t+1 d'un arbre
 * @author Florian et Ugo
 * @param ab
 * @deprecated
 */
void Foret::transition(Arbre* ab)
{
	unsigned distAdj= 1; // distance à laquelle les voisins seront enflammes ; IMPROVEIT dist 1= contact, dist 2= diagonaux, dist 3 = ...
	list< Arbre* > voisins= adjacents(ab, distAdj);
	for (list< Arbre* >::iterator a(voisins.begin()); a!=voisins.end(); ++a){
		// intensite fixe		
		spark( (*a), 5 );
	}
	
	if ( ab->burn(burningCoef) )
		onFire.push_back(ab);
	else // quand un arbre ne brule plus (il devient cendres), on l'ajoute à la liste des arbres devenus cendres
		carbonized.push_back(ab);
}


/**
 * Parcours la liste des arbres en feu et emflamme les voisins en tenant compte de la direction du vent
 * @author Ugo
 * 
 */
void Foret::transitionWind(Arbre* a, const Vent* vent)
{
	burnAdjacentsWind(a, vent);
	
	if ( a->burn(burningCoef) )
		onFire.push_back(a);
	else // quand un arbre ne brule plus (il devient cendres), on l'ajoute à la liste des arbres devenus cendres
		carbonized.push_back(a);
}


/**
 * passe de t à t+1 tous les arbres à l'aide de la liste d'arbres en feu
 * @author Florian et Ugo
 * @return vrai si il y eu des changements, faux sinon
 */
bool Foret::NextMove()
{
	bool modif(false);
	
	if (!onFire.empty()){
		modif= true;
		list< Arbre* > old= onFire;
		onFire.clear();		
		
		#if DEBUG_SPARK
		cout<< endl<< "TOUR DE TRANSMISSION"<< endl;
		#endif
		// TODO utiliser mapping ?
		for (list< Arbre* >::iterator ab(old.begin()); ab!=old.end(); ++ab){
// 			transition(*ab);
			transitionWind(*ab, wind);
		}
	}
	
	return modif;
}


// ################################
//		Persistance des donnees
// ################################

void Foret::loadEssences(ifstream* file)
{
	int nbEssences;
	file->read( (char*)&(nbEssences), sizeof(int));
#if DEBUG_LOAD
	cout<< "Nombre essences : " << nbEssences<< endl;
#endif
	
	for(int i=0; i<nbEssences; ++i){
		char nom[40];
		int matur;
		int diam;
		int mass;
		int haut;
		unsigned age;
		bool type;
		
		file->read( (char*)&(nom), 40*sizeof(char));
		file->read( (char*)&(matur), sizeof(int));
		file->read( (char*)&(diam), sizeof(int));
		file->read( (char*)&(mass), sizeof(int));
		file->read( (char*)&(haut), sizeof(int));
		file->read( (char*)&(age), sizeof(unsigned));
		file->read( (char*)&(type), sizeof(bool));
		
		string name(nom);
		
		Essence e(i, nom, mass, diam, haut, age, type);
		cout<< e.toString()<< endl;
		
		essences.push_back(e);
	}
}


void Foret::loadMatrix(ifstream* file)
{
	cout<< "Chargement ..."<< endl;
	file->read( (char *)&(colonnes), sizeof(int));
	file->read( (char *)&(lignes), sizeof(int));
	#if DEBUG_LOAD
	cout<< "Taille : " << colonnes<< " en largeur "<<lignes<< " en hauteur" <<endl;
	#endif

	initEmpty();
	
	// Arbres
	while(!file->eof()){
		int col='a', row='b';
		file->read((char *)&col, sizeof(int));
		file->read((char *)&row, sizeof(int));

		unsigned indice;
		
		file->read( (char*)&(indice), sizeof(unsigned));
		
		plantTree(col, row, indice);
		
		#if DEBUG_LOAD
// 		cout<< "arbre en : "<< col<< "; "<< row << " essence indice : " << indice<<endl;
		#endif
	}
// 		int progression= 0;
}

bool Foret::load(string fileName)
{
	string chemin= "./Resources/"+fileName+".dat";
	ifstream file(chemin.c_str(), ios::in|ios::binary);
	// 	file.
	if (!file.is_open()){
		cout<< "Echec ouverture fichier de sauvegarde"<< endl;
		return false;
	}
	else {
		loadEssences(&file);
// 		loadEssences("../Moteur/essence_data.txt");
		loadMatrix(&file);
		
		file.close();
		
		return true;
	}
}


void Foret::saveEssences(ofstream* file)
{
	int taille= essences.size();
	file->write( (char*)&(taille), sizeof(int));
	
	for (vector<Essence>::iterator e(essences.begin()); e != essences.end(); ++e){

		const char* nom(e->getName().c_str());
		int matur= e->getAgeMaturite();
		int diam= e->getDiametre();
		int mass= e->getMasse();
		int haut= e->getHauteur();
		unsigned age;
		bool type;
		
		file->write( (char*)&(nom), 40*sizeof(char));
		file->write( (char*)&(matur), sizeof(int));
		file->write( (char*)&(diam), sizeof(int));
		file->write( (char*)&(mass), sizeof(int));
		file->write( (char*)&(haut), sizeof(int));
		file->write( (char*)&(age), sizeof(unsigned));
		file->write( (char*)&(type), sizeof(bool));		
	}
}

void Foret::saveMatrix(ofstream* file)
{
	file->write( (char *)&(colonnes), sizeof(int));
	file->write( (char *)&(lignes), sizeof(int));
	
	int progression= 0;
	
	for (int i= 0; i< lignes; ++i){
		for(vector< Cellule* >::const_iterator a(matrix[i].begin()); a != matrix[i].end() ; ++a ){
			if ( (*a)->getState()>0){
				Arbre * ab= dynamic_cast< Arbre * >(*a);
#if DEBUG_SAVE
cout<< "Enregistrement de l'arbre "<< ab->getPos().col<< "; "<< ab->getPos().row<< endl; 
#endif
					
				// Position
// 						file<< ab->getPos().col << ab->getPos().row;
				
				// PV
// 						file<< ab->getPv();
				
				// Position
				file->write( (char *)&(ab->getPos().col), sizeof(int));
				file->write( (char *)&(ab->getPos().row), sizeof(int));
				
				// PV
// 					file.write( (char *)ab->getPv(), sizeof(int));
				
				unsigned indice= ab->getEssence()->getIndice();
				
				file->write( (char*)&(indice), sizeof(unsigned));
			}
		}
		
		int newProgression= i*100 / lignes;
		while (newProgression>progression){
			cout<< "=";
			progression++;
		}
		cout.flush();
	}
	cout<< endl<< progression<<"%"<< endl;

}

bool Foret::save(string fileName)
{
	string chemin= "./Resources/"+fileName+".dat";
	ofstream file(chemin.c_str(), ios::out|ios::binary);
	
	if (!file.is_open()){
		cout<< "Echec ouverture fichier de sauvegarde"<< endl;
		return false;
	}
	else {
		cout<< "Sauvegarde ..."<< endl;
		
		saveEssences(&file);
		saveMatrix(&file);
		
		file.close();
		
		return true;
	}
}


// #########################
// #	Affichage attributs	#
// ######################### 
void Foret::showEssences() const
{
	cout << "| Nom\t\t||"<< " indice\t||"<< " masseV\t||"<< " diam\t||"<< " haut\t||"<< " type\t||"<< " ageM\t|"<< endl;
	cout << "-------------------------------------------------------------------------------"<< endl;
	
	for(vector<Essence>::const_iterator i(essences.begin()); i!=essences.end(); ++i){
		string nom= i->getName();
		if (nom.size()<=9)
			nom+="\t";
		
		cout << "| "<< nom<< "\t| "<< 
		i->getIndice()		<< "\t| "<<
		i->getMasse()		<< "\t| "<<
		i->getDiametre()	<< "\t| "<< 
		i->getHauteur()	<< "\t| "<<
		i->getType() 		<< "\t|"<<
		i->getAgeMaturite()	<< "\t|"<< endl;
	}
}