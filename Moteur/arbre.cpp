#include "arbre.h"

/*
 * On considère pour la gestion de l'embrasement d'un arbre
 * et pour sa combustion le système suivant :
 * Le taux d'humidité de l'arbre diminue au fil de sa combustion
 * Plus ce taux est faible, plus l'arbre perd de points de vie par tour
 * De plus, des paramètres extérieurs pourront ralentir cette perte ( pluie, humidité ambiante, canadarage, etc ^^)
 * 
 * On considère de plus qu'un arbre en feu ne génère pas de flamme tant que son humidité n'est pas inférieure à 50% FAUX
 * Par conséquent, un arbre emflammera son voisin uniquement à cette condition
 */

using namespace std;

Arbre::Arbre(int col, int row, const Essence* const _essence, unsigned _age, unsigned _humidite)
	: Cellule(1), pos(col, row),essence(_essence),age(_age),humidity(_humidite)
{	
	initialise();
}

/**
 * Construit un arbre "à partir" d'une cellule
 * @param cell cellule à remplacer, elle est désallouée
 */
Arbre::Arbre(Cellule* cell, int col, int row, const Essence*const _essence, unsigned int _age, unsigned int _humidite)
	: Cellule(1), pos(col, row),essence(_essence),age(_age),humidity(_humidite)
{
	initialise();
	// suppression ancienne cellule 
	delete(cell);
}


/**
 * Initialise les points de vie de l'arbre en fonction des caractères de son essence
 * et de ses propriétés discrètes
 * @author Ugo Florian
 */
void Arbre::initialise()
{
	/*
	 * Nous avons décidé d'établir le rayon et la hauteur de l'arbre en fonction de son âge
	 * et de l'âge de maturité de son essence, en simulant des variations de croissance
	 * par l'utilisation de tranche d'âge où la croissance varie. Par défaut, nous utiliserons
	 * trois tranches :
	 * [0;1/4 age maturité[
	 * [1/4;3/4 age maturité[
	 * [3/4; age maturité [
	 * 
	 * Si un arbre est plus vieux que son âge de maturité, il possèdera les caractères
	 */
	// Initialisation des points de vie de l'arbre
	#if DEBUG_PV
	cout << "Essence :  "<< essence->toString()<< endl ;
	#endif
	
	float rayon = essence->getDiametre() / 2.0;
	float	hauteur = essence->getHauteur();
	float masseV= essence->getMasse()/1000.0;
	
// 	int ageMatur= essence->getAgeMaturite();
// 	unsigned points;
// 	if (age>= ageMatur)
// 		points= ageMatur /* *croissance/an ?*/;
// 	else {
// 		// points avant maturité :
// 		unsigned pointsJ= ageMatur /* *croissance/an ?*/;
// 		points= pointsJ + (age-ageMatur) /* *croissance/an ?*/;
// 	}
	
	unsigned points = (unsigned) ( (M_PI* (rayon*rayon)*hauteur) * masseV );
	
	// Les données des essences sont décrites pour un arbre de 50 ans à 20% d'humidité

	hp = points *(1 +( 50 + (age/*-ageMatur*/ ) )/50.0);
	hp *= 1  + (humidity-12)/100.0;
	
	#if DEBUG_PV==1
	cout << " => hp : "<< hp << endl;
	#endif
	// L'initialisation du coefficient sera faite plus tard puisqu'il dépend des paramètres extérieurs à l'arbre 
	// Humidité ambiante, force du vent etc
	 coefficient = 1.0;
}
/*** Setter ***/
void Arbre::setCoefficient(float x)
{
	coefficient = x;
}

// ###############
// 	Actions
// ###############

/**
 * L'arbre reçoit des flammes et risque de s'enflammer
 * @author Florian
 */
void Arbre::spark(float coef)
{
	int taux= 10*coef/humidity *100;
	
	int aleatoire= rand()%101;
	
	#if DEBUG_SPARK
	cout<< "enflammer humidite "<< humidity<<" ? taux: "<< taux<< " rand "<< aleatoire<< " : ";
	#endif
	
	if (taux > aleatoire){
		state= 2;
		#if DEBUG_SPARK
		cout<< "oui"<< endl;
		#endif
	}
	else {
		humidity -= coef;
		#if DEBUG_SPARK
		cout<< "non"<< endl;
		#endif
	}
}


/**
 * Retire un nombre de points de vie à l'arbre
 * @author Florian
 * @param coef
 */
bool Arbre::burn(float coef)
{
	// On va déterminer le nombre de points de vie à enlever.
	int decrementation = 100.0*coef*coefficient;
	
	decrementation *= 1.0/( (float)humidity );
	#if DEBUG_DECRE
	cout << coef<< ",decre init: "<< decrementation << "|";
	cout << "type  : " << essence->getType();
	#endif
	
	if(essence->getType() == 0){
		// On considère qu'un épineux brule plus vite qu'un feuillu
		decrementation *= 1.20;
		#if DEBUG_DECRE
		cout << "epin: "<< decrementation << "-"<< endl;
		#endif	
	}
	#if DEBUG_DECRE
	cout << ",fin: "<< decrementation << "||";
	#endif
	
	hp-= decrementation;
	humidity *= 0.9;
	// Si l'arbre n'a plus de points de vie, il passe à l'état cendre
	if (hp<=0){
		blast();
		return false;
	}
	else return true;
}
