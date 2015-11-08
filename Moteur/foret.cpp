#include "foret.h"

// Valeur du nombre pi, utilisée pour les calcul de trigonométrie
#define PI 3.14159265
#define ESSENCE_PATH "../Resources/essence_data.txt"

// ages et humidité min et max
int hMin= 20;
int ecartHMax= 50;
int ecartAgeMax= 80;

using namespace std;

// ###################################
//		Constructeurs et destructeur
// ################################### 
Foret::Foret(int _largeur, int _hauteur, float proba, float _coefFeu, time_t graine)
: lignes(_hauteur), colonnes(_largeur), burningCoef(_coefFeu), randomSeed(graine)
{
	if (tryLoadEssences(ESSENCE_PATH)){
	    randomMatrix(proba);
	    
	    wind = new Vent();  
	}
	else {
	    cerr << "Impossible de charger le fichier d'essences"<< endl
	    << "FIN DU PROGRAMME"<< endl;
	    exit(0);
	}    
	
// 	else throw (exception); // TODO redefinir l'exception
}

Foret::Foret(int _largeur, int _hauteur, ifstream * file, LoadProgress* PB)
	:lignes(_hauteur), colonnes(_largeur)
{
	wind = new Vent();
	tryLoad(file, PB);
}

Foret::Foret(int _largeur, int _hauteur, vector< std::vector< int > >* matrice,float coef_brulure)
	: lignes(_hauteur), colonnes(_largeur), burningCoef(coef_brulure)
{
	wind = new Vent();
	tryLoadEssences(ESSENCE_PATH);
	#if DEBUG_ARBRE_PLANTE
	cout<< "création de la foret à partir d'une matrice d'intensité, de taille "<< _largeur<< "x"<< _hauteur<< endl;
	#endif
	create(_largeur, _hauteur, matrice);
}


Foret::~Foret()
{
	for (int i= 0; i< lignes; ++i){
		for (vector< Cellule* >::iterator j( matrix[i].begin() ); j!=matrix[i].end(); ++j){
			delete *j;
		}
	}
	
	// a priori, inutile de vider les listes
	onFire.clear();
	burned.clear();
	carbonized.clear();
	delayBurned.clear();
	delayed.clear();
	uprooted.clear();
	
	delete wind;
}


// ########################
//		Initialisations
// ########################
void Foret::initEmpty()
{
	for (int i= 0; i< lignes; ++i){
		// création d'une nouvelle "ligne de la matrice"
		std::vector< Cellule* > tmp;
		
		for (int j= 0; j< colonnes; ++j){
			tmp.push_back( new CelluleVide );
		}
		
		// ajout de la ligne dans la matrice
		matrix.push_back(tmp);
	}
}


vector< string >& Foret::explode(const string& str)
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

// ################
// 	Setters
// ################
void Foret::setValues(int largeur, int hauteur, float coef)
{
	lignes = hauteur;
	colonnes = largeur;
	burningCoef = coef;
}

void Foret::setWind(int angle, int vitesse)
{
	#if DEBUG_VENT
	cout << "valeur de l'angle dans firescreen : " << angle << endl;
	#endif
	
	float vertical = sin(PI*(float)(angle)/180.0);
	float horizontal = cos(PI*(float)(angle)/180.0);
	#if DEBUG_HYPO
	cout <<"cosinus de l'angle envoyé : " << horizontal << endl;
	cout <<"sinus de l'angle envoyé : " << vertical << endl;
	#endif
	
	vertical *= vitesse/20.0;
	horizontal *= vitesse/20.0;
	#if DEBUG_HYPO
	cout <<"deplacement horizontal en pixel: " << horizontal << endl;
	cout <<"deplacement vertical en pixel : " << vertical << endl;
	#endif
	
	// correction des valeurs si elles ont dépassés les seuils maximaux
	if(horizontal < 1 && horizontal > 0)
		horizontal = 1;
	else if(horizontal > -1 && horizontal < 0)
		horizontal = -1;
	else if(vertical < 0 && vertical > -1)
		vertical = -1;
	else if(vertical > 0 && vertical < 1)
		vertical =1;
	
	wind->setPower_h(horizontal);
	wind->setPower_v(vertical);
	#if DEBUG_VENT or DEBUG_HYPO
	cout << "valeur power h" << horizontal << endl;
	cout << "valeur power v" << vertical << endl;
	#endif
}


bool Foret::tryLoadEssences(const string& fileName)
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
			float diam; istringstream(tokens[2])>> diam;
			float	haut;	istringstream(tokens[3])>> haut;
			essences.push_back( Essence(indice, tokens[0],
											atoi(tokens[1].c_str()),
											diam,
											haut,
											atoi(tokens[4].c_str()),
											atoi(tokens[5].c_str())		) );
			
			indice +=1;
			// suppression du vecteur de strings instancié en pointeur
			delete &tokens;
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

std::list< list< Arbre* > >* Foret::getChanged()
{
	list< list< Arbre* > >* listes= new list< list< Arbre* > >();
	listes->push_back(uprooted);
	listes->push_back(delayed);
	listes->push_back(delayBurned);
	listes->push_back(burned);
	listes->push_back(carbonized);
	return listes;
}


void Foret::plantTree(int col, int row)
{
	#if DEBUG_ARBRE_PLANTE
	cout<< "\t"<< col<< endl;
	#endif
	
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

void Foret::plantTree(int col, int row, unsigned int numEss, int PdV, float humidite, float coef, int etat)
{
	#if DEBUG_ARBRE_PLANTE
	cout<< "arbre planté en "<< col << ";"<< row<< endl;
	#endif	
	
	const Essence* pEss= &(essences[numEss]);
	// pour l'instant, on considere que tous les arbres ont atteint leur maturite
	Arbre* ab= new Arbre(col,row, matrix[row][col], pEss, humidite, PdV);
	
	if (coef < 1.0)
		delay(ab, coef);
	
	if (etat==-1)
		blast(ab);
	else if (etat==-2)
		delay(ab);
	else if (etat==2)
		kindle(ab);
	
	matrix[row][col]= ab;
}

// IMPROVEIT Il faudrait pouvoir choisir une sensibilité au vert ou à la luminosité, ou bien calculer la luminosité moyenne de l'image pour la prendre en compte
// 	On peut avoir des résultat étranges en changeant la luminosité ou l'etalonnage d'une même image
void Foret::create(int largeur, int hauteur, vector< vector< int > >* matrice)
{
	initEmpty();
#if DEBUG_CURRENT
	cout << "ajout des arbres si intensité vert > autres"<< endl;
#endif
	
	for(int ligne= 0; ligne< hauteur; ++ligne){
		#if DEBUG_ARBRE_PLANTE
		cout<< "plantage des arbres de la ligne : "<< ligne<< endl;
		#endif
		
		for (int colonne=0; colonne< largeur; ++colonne){
			if ( (*matrice)[ligne][colonne] > 50){
				plantTree(colonne, ligne);
			}
		}
	}
}

void Foret::randomMatrix(float probabilite)
{
	srand(randomSeed);
	
	if (probabilite>1){
		probabilite=0.6;
		std::cout << "MIS A DEFAUT"<< endl;
	}
	
	// Initialisation de l'ensemble des cellules à vide
	initEmpty();
	
	// Arbres placés aléatoirement
	for (int i= 0; i< lignes; ++i){
		#if DEBUG_ARBRE_PLANTE
		cout<< "plantage des arbres de la ligne : "<< i<< endl;
		#endif
		
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


void Foret::clearChanged()
{
	uprooted.clear();
	delayed.clear();
	delayBurned.clear();
	burned.clear();
	carbonized.clear();
}


// ##################################
// 	Modification des éléments
// ##################################

// IMPROVEIT couper un arbre : supprimer arbre puis créer cellule ?, laisser l'arbre ? (pour l'instant l'arbre est laissé)
void Foret::uproot(Arbre* ab)
{
	ab->uproot();
	onFire.remove(ab);
	uprooted.push_back(ab);
}

void Foret::uproot(int col, int row)
{
	Cellule* tmp= matrix[row][col];
	
	if (tmp->getState()>0){	// si il s'agit un arbre
		Arbre * ab = dynamic_cast<Arbre *>(tmp);
		uproot(ab);
	}
}

void Foret::delay(Arbre* ab, float coef)
{
	ab->delay(coef);
	delayed.push_back(ab);
}

// EMBRASEMENT
void Foret::kindle(Arbre* ab)
{
	ab->kindle();
	onFire.push_back(ab);
	
	if (ab->getCoeff() < 1)
		delayBurned.push_back(ab);
	else 
		burned.push_back(ab);
}

void Foret::kindle(int col, int row)
{	
	if (matrix[row][col]->getState() ==1){
		Arbre * ab = dynamic_cast<Arbre *>(matrix[row][col]);
		kindle(ab);
	}
}

// EXTINCTION
void Foret::blast(Arbre* ab)
{
	ab->blast();
	carbonized.push_back(ab);
}


// BURNING
void Foret::spark(Arbre* ab, int intensite)
{
// NOTE : on pourrait augmenter 	la brulure de l'arbre quand un de ses voisins lui transmet du feu, voir un concept de chaleur.
// 	On pourrait egalement aller plus loin et introduire une antité feu, une humidite de zone qui évolue selon la quantité de feu..
// 	Dans ce cas, il faudrait prendre en compte différentes tailles d'arbres pour être précis ...
// 	On a décidé de se restreindre sur ce point afin de se concentrer sur d'autre points.
// 	if (ab->getState==2){
// // 		ab->brule();
// 	}
// 	else {
	ab->spark(burningCoef*intensite);
	if (ab->isOnFire()){
		onFire.push_back(ab);
		
		if (ab->getCoeff() < 1)
			delayBurned.push_back(ab);
		else 
			burned.push_back(ab);
	}
}

void Foret::spark(int col, int row, int intensite)
{	
	if (matrix[row][col]->getState()==1){
		Arbre * ab = dynamic_cast<Arbre *>(matrix[row][col]);
		spark(ab, intensite);
	}
}


//###########################
// 	Altération en zone
//###########################
void Foret::cut(int xDep, int yDep, int xArr, int yArr)
{
	for(int i= xDep; i < xArr; ++i){
		
		for(int j= yDep; j < yArr; ++j){
// 			matrix[j][i]->setState(-2);
			Cellule* cell= matrix[j][i];
			if (cell->getState()>0)
				uproot(  dynamic_cast<Arbre *>(matrix[j][i]) );
		}
	}
}

void Foret::delay(int xDep, int yDep, int xArr, int yArr)
{
	for(int i= xDep; i < xArr; ++i){
		
		for(int j= yDep; j < yArr; ++j){
			// Le retardateur ne s'applique que sur les arbres
			Cellule* cell= matrix[j][i];
			if (cell->getState()>0)
				// Réduit le coefficient de combustion personnel de l'arbre à 0.5
				delay(  dynamic_cast<Arbre *>(cell) );
		}
	}
}


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

std::list< Arbre* > Foret::adjacents(const Arbre * ab, int distance) const
{
	return adjacents(ab->getPos().col,ab->getPos().row, distance);
}


void Foret::sparkAdjacentsWind(int posCol, int posRow, int hor, int vert)
{
	int x= 1, y= 1;
	
	// définition du signe de l'incrémentation/décrémentation
	if(hor<0)
		x= -1;
	if(vert<0)
		y= -1;
	
	// Definition de la distance à laquelle les arbres qui sont dos au vent sont enflammés
		// Si le vent est supérieur à "2 cases", seuls les arbres faces au vent sont enflammés
	int inverseHor= x;
	int inverseVert= y;
	unsigned forceHor= abs(hor);
	if(forceHor < 2)
		inverseHor*= 2;
	else if (forceHor>2)
		inverseHor= 0;
	
	unsigned forceVert= abs(vert);
	if(forceVert < 2)
		inverseVert*= 2;
	else if (forceVert>2)
		inverseVert= 0;
		
	// On parcourt les cellules dans les sens du vent et 1 cellule de distance à l'opposé du vent (tranmission "arrière")
	for(int i = hor+x; i != -inverseHor; i-=x) {
		for(int j = vert+y; j != -inverseVert; j-=y) {

			// Test d'appartenance des coordonnées à la matrice
			if( ( (posCol+i) >= 0 ) && ( (posCol+i) < (colonnes) ) && ( (posRow+j) >= 0 ) && ( (posRow+j) < (lignes) ) ){
				#if DEBUG_VENT2
				cout << "transmission à cellule en "<< posRow + (hor -i)<< " ; "<< posCol + (vert -j)<< endl;
				#endif
				// transmission du feu à la cellule voisine si c'est un arbre
				Cellule* cell= matrix[posRow+j][posCol+i];
				if(cell->getState() == 1)
					// voir si on met abs(i) et abs(j)
					spark(dynamic_cast < Arbre* >(cell), (abs(hor-i)+1)*(abs(vert-j)+1) );
			}
		}
	}
}

void Foret::sparkAdjacentsWind(Arbre* a, const Vent* vent)
{
	#if DEBUG_VENT2
	cout<< endl<< "BRULE LES ADJACENTS DE "<< a->getPos().col<< ";" << a->getPos().row<< endl;
	#endif

	sparkAdjacentsWind(a->getPos().col, a->getPos().row,
						vent->getPower_h(), vent->getPower_v() );
}


// #############################
// #		Avancée du temps		  #
// #############################
void Foret::transition(Arbre* ab)
{
	unsigned distAdj= 1; // distance à laquelle les voisins seront enflammes VOIR adjacents
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

void Foret::transitionWind(Arbre* a, const Vent* vent)
{
	sparkAdjacentsWind(a, vent);
	
	if ( a->burn(burningCoef) )
		onFire.push_back(a);
	else // quand un arbre ne brule plus (il devient cendres), on l'ajoute à la liste des arbres devenus cendres
		carbonized.push_back(a);
}


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
// 			transition(*ab); // sans prendre en compte le vent
			transitionWind(*ab, wind);
		}
	}
	
	return modif;
}


// ################################
//		Persistance des donnees
// ################################

void Foret::loadSizes(ifstream* file)
{
	file->read( (char *)&(colonnes), sizeof(int));
	file->read( (char *)&(lignes), sizeof(int));
	#if DEBUG_LOAD
	cout<< "Taille : " << colonnes<< " en largeur "<<lignes<< " en hauteur" <<endl;
	#endif
}

void Foret::loadBurningCoef(ifstream* file)
{
	file->read( (char *)&(burningCoef), sizeof(float));
	#if DEBUG_LOAD
	cout<< "Coefficient de brulure : "<< burningCoef<< endl;
	#endif
}

void Foret::loadEssences(ifstream* file)
{
	int nbEssences;
	file->read( (char*)&(nbEssences), sizeof(int));
#if DEBUG_LOAD
	cout<< "Nombre essences : " << nbEssences<< endl;
#endif
	
	for(int i=0; i<nbEssences; ++i){
		unsigned indice;
		char nom[50];
// 		const char* nom;
		unsigned mass;
		float diam;
		float haut;
		unsigned ageMatur;
		bool type;
		
		file->read( (char*)&(indice),	sizeof(unsigned));
		file->read( (char*)&(mass),	sizeof(int));
		file->read( (char*)&(nom),		50*sizeof(char));
		file->read( (char*)&(diam),	sizeof(float));
		file->read( (char*)&(haut),	sizeof(float));
		file->read( (char*)&(ageMatur),	sizeof(unsigned));
		file->read( (char*)&(type),	sizeof(bool));
		
		cout << "nom : "<< nom<< ";end"<< endl;
		
		Essence e(indice, (string)nom, mass, diam, haut, ageMatur, type);
		#if DEBUG_LOAD
		cout<< e.toString()<< endl;
		#endif
		
		essences.push_back(e);
	}
}

void Foret::loadMatrix(ifstream* file, LoadProgress* progress)
{
	#if DEBUG_LOAD
	cout<< "Chargement ..."<< endl;
	#endif
	
	initEmpty();
	#if DEBUG_LOAD
	cout<< "fin init foret"<< endl;
	#endif
	
	// Arbres
	while(!file->eof()){
		int col, row;
		file->read((char *)&col, sizeof(int));
		file->read((char *)&row, sizeof(int));

		unsigned indice;
		file->read( (char*)&(indice), sizeof(unsigned));
		
		// PV et Humidité
		int PV;
		float humidite;
		file->read((char*)&(PV), sizeof(int));
		file->read((char*)&(humidite), sizeof(float));
		
		// Coef brulure
		float coefBrulure;
		file->read((char*)&(coefBrulure), sizeof(float));
		
		int etat;
		file->read( (char*)&(etat), sizeof(int));
		
		plantTree(col, row, indice, PV, humidite ,coefBrulure, etat);
		#if DEBUG_LOAD_POS
		cout<< "arbre en : "<< col<< "; "<< row << " essence indice : " << indice<< endl;
		#endif
		
		int newProgression= (row*100) / (lignes);
		progress->setProgress(newProgression);
		#if DEBUG_PROGRESS
		cout<< "progression : "<< newProgression<< "%"<< endl;
		#endif
	}
}

bool Foret::tryLoad(ifstream* file, LoadProgress* progress)
{
	if (!file->is_open()){
		cout<< "Echec ouverture fichier de sauvegarde"<< endl;
		return false;
	}
	else {
		loadBurningCoef(file);
		loadEssences(file);
		loadMatrix(file, progress);
		
		file->close();
		
		return true;
	}
}

// SAUVEGARDES
void Foret::saveProperties(ofstream* file)
{
	file->write( (char *)&(colonnes), sizeof(int));
	file->write( (char *)&(lignes), sizeof(int));
	
	file->write( (char *)&(burningCoef), sizeof(float));
}

void Foret::saveEssences(ofstream* file)
{
	int taille= essences.size();
	file->write( (char*)&(taille), sizeof(int));
	
	for (vector<Essence>::iterator e(essences.begin()); e != essences.end(); ++e){

		unsigned indice= e->getIndice();
// 		char nom[40];
// 		const char* nom(e->getName().c_str());
// 		name.assign(e->getName().c_str(), 40);

		string name(e->getName().c_str());
		char* nom= new char[50];
		unsigned i;
		for (i= 0; i<name.length(); ++i ){
			nom[i]= name[i];
		}
		for ( ; i<49; ++i)
			nom[i]=' ';
			
		nom[49]= '\0';
		cout << nom<< "; end"<< endl;
		
		unsigned mass= e->getMasse();
		float diam= e->getDiametre();
		float haut= e->getHauteur();
		unsigned ageMatur= e->getAgeMaturite();
		bool type= e->getType();
		file->write( (char*)&(indice),	sizeof(unsigned));
// 		file->write( (char*)&(name),	sizeof(string));
		file->write( (char*)&(mass),	sizeof(int));
		file->write( (char*)&(nom),		50*sizeof(char));
		file->write( (char*)&(diam),	sizeof(float));
		file->write( (char*)&(haut),	sizeof(float));
		file->write( (char*)&(ageMatur),	sizeof(unsigned));
		file->write( (char*)&(type),	sizeof(bool));		
	}
}

void Foret::saveMatrix(ofstream* file)
{
	int progression= 0;
	
	for (int i= 0; i< lignes; ++i){
		for(vector< Cellule* >::const_iterator cell(matrix[i].begin()); cell != matrix[i].end() ; ++cell ){
			if ( (*cell)->getState()!=0){
				Arbre * ab= dynamic_cast< Arbre * >(*cell);
#if DEBUG_SAVE_POS
cout<< "Enregistrement de l'arbre "<< ab->getPos().col<< "; "<< lignes<< endl;
#endif

				// Position
// 				file<< ab->getPos().col << ab->getPos().row;
				
				// Position
				file->write( (char *)&(ab->getPos().col), sizeof(int));
				file->write( (char *)&(ab->getPos().row), sizeof(int));
				
				// Essence
				unsigned indice= ab->getEssence()->getIndice();
				file->write( (char*)&(indice), sizeof(unsigned));
				
				// PV et Humidité
				int PV= ab->getPv();
				float humidite= ab->getHumidity();
				file->write((char*)&(PV), sizeof(int));
				file->write((char*)&(humidite), sizeof(float));
				
				// Coef brulure
				float coefBrulure= ab->getCoeff();
				file->write((char*)&(coefBrulure), sizeof(float));
				
				// Etat
				int etat= ab->getState();
				file->write( (char*)&(etat), sizeof(int));
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


bool Foret::trySave(string fileName)
{
	string chemin= fileName+".dat";
	ofstream* file= new ofstream(chemin.c_str(), std::ios::out|std::ios::binary);
	
	if (!file->is_open()){
		cout<< "Echec ouverture fichier de sauvegarde "<< fileName<< endl;
		return false;
	}
	else {
		saveProperties(file);
		saveEssences(file);
		saveMatrix(file);
		cout<< "Sauvegarde dans "<< fileName<< " réussie"<< endl;
		
		file->close();
		
		return true;
	}
}

bool Foret::trySaveSeed(string filePath)
{
	ofstream* file= new ofstream(filePath.c_str(), std::ios::out|std::ios::binary);
	
	if (!file->is_open()){
		cout<< "Echec ouverture fichier de sauvegarde : " << filePath<< endl; // TODO émettre une alerte à l'utilisateur
		return false;
	}
	else {
	#if DEBUG_SAVE
	  cout<< "Sauvegarde de la graine réussie dans le fichier " << filePath<< endl;
	#endif
	  
	file->write( (char*)&(randomSeed), sizeof(time_t));
	
	saveProperties(file);

	file->close();
	
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