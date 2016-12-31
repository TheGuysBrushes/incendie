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

Arbre::Arbre(int col, int row, const Essence* const _essence, unsigned age, float _humidite, float _coefficient)
    : Cellule(), state(1), pos(col, row), essence(_essence), humidity(_humidite), coefficient(_coefficient)
{
    initialise(age);
}

Arbre::Arbre(Cellule* cell, int col, int row, const Essence*const _essence, unsigned age, float _humidite, float _coefficient)
    : Cellule(), state(1), pos(col, row), essence(_essence), humidity(_humidite), coefficient(_coefficient)
{
    initialise(age);
    // suppression ancienne cellule
    delete(cell);
}

Arbre::Arbre(int col, int row, Cellule* cell, const Essence* const _essence, float _humidite, int _hp, float _coefficient)
    : Cellule(), state(1), pos(col, row), essence(_essence), humidity(_humidite), coefficient(_coefficient), hp(_hp)
{

    // suppression ancienne cellule
    delete(cell);
}


void Arbre::initialise(unsigned age)
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
    // 	int ageMatur= essence->getAgeMaturite();
    // 	unsigned points;
    // 	if (age>= ageMatur)
    // 		points= ageMatur /* *croissance/an ?*/;
    // 	else {
    // 		// points avant maturité :
    // 		unsigned pointsJ= ageMatur /* *croissance/an ?*/;
    // 		points= pointsJ + (age-ageMatur) /* *croissance/an ?*/;
    // 	}

    // Initialisation des points de vie de l'arbre
#if DEBUG_PV
    cout << "Essence :  "<< essence->toString()<< endl ;
#endif

    float rayon = essence->getDiametre() / 2.0f;
    float hauteur = essence->getHeight();
    float masseV= essence->getMasse()/1000.0f;


    unsigned points = (unsigned) ( (M_PI* (rayon*rayon)*hauteur) * masseV );

    // Les données des essences sont décrites pour un arbre de 50 ans à 20% d'humidité

    hp = points * (unsigned)(1 +( 50 + (age/*-ageMatur*/ ) )/50.0f);
    hp *= 1  + (unsigned)((humidity-12)/100.0f);

#if DEBUG_PV==1
    cout << " => hp : "<< hp << endl;
#endif
}

/*** Setter ***/
void Arbre::setCoefficient(float x)
{
    coefficient = x;
}

void Arbre::setState(int x){
    state= x;
}

void Arbre::delay(float coefRalentissement)
{
    setCoefficient(coefficient*coefRalentissement);
}


// ###############
// 	Actions
// ###############
void Arbre::spark(float coefTransmission)
{
    float taux= (coefTransmission*coefficient)/humidity *100;

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
        humidity -= coefTransmission*coefficient;
#if DEBUG_SPARK
        cout<< "non"<< endl;
#endif
    }
}


bool Arbre::burn(float coefBrulure)
{
    // On va déterminer le nombre de points de vie à enlever.
    float decrementation = 100.0f*coefBrulure*coefficient;

    // Diminution de la brulure selon l'humidité
    decrementation *= 1.0f / ( (float)humidity );
#if DEBUG_DECRE
    cout << coef<< ",decre init: "<< decrementation << "|";
    cout << "type  : " << essence->getType();
#endif

    if(essence->getType() == 0){
        // On considère qu'un épineux brule plus vite qu'un feuillu
        decrementation *= 1.20f;
#if DEBUG_DECRE
        cout << "epin: "<< decrementation << "-"<< endl;
#endif
    }
#if DEBUG_DECRE
    cout << ",fin: "<< decrementation << "||";
#endif

    hp-= (unsigned)decrementation;
    humidity *= 0.9f;
    // Si l'arbre n'a plus de points de vie, il passe à l'état cendre
    if (hp<=0){
        blast();
        return false;
    }
    else return true;
}
