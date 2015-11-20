#ifndef FORET_H
#define FORET_H

#include <QtWidgets/QProgressBar> // IMPROVEIT ?

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
// Pour l'aléatoire
#include <cstdlib>
#include <ctime>

#include "../debug.h"
#include "cellulevide.h"
#include "essence.h"
#include "arbre.h"
#include "vent.h"
#include "loadprogress.h"

#define MAXI 1000 

class Foret
{
private:
	int lignes;
	int colonnes;
	
	float burningCoef; // ralenti la progression du feu, il lui faut plus de tours pour avancer (peut affecter la précision des mesures)
	
	std::time_t randomSeed;
	
	Vent* wind;
	std::vector< Essence > essences;
	std::vector<std::vector< Cellule* > > matrix;
	
	std::list< Arbre* >	onFire;
	std::list< Arbre* >	uprooted;
	std::list< Arbre* >	delayed;
	std::list< Arbre* >	delayBurned;
	std::list< Arbre* >	burned;
	std::list< Arbre* >	carbonized;
	
// METHODES
public:
// Constructeur et destructeur
	/**
	 * Constructeurs de forêt aléatoirement, à partir de paramètres
	 * @author Florian
	 * 
	 * @param _largeur nombre de colonnes de la matrice représentant la forêt, sa largeur
	 * @param _hauteur nombre de lignes de la matrice, la hauteur
	 * @param proba, probabilite qu'il y ait un arbre, pour chaque case de la matrice. C'est environ le pourcentage d'arbres
	 * @param _coefFeu coefficient de propagation du feu : 1 forêt "classique"; <1 progression plus lente (humidité...); >1 progression plus rapide (sécheresse ?)
	 */
	Foret(int _largeur, int _hauteur, float proba = 0.60, float _coefFeu = 0.5, std::time_t graine=std::time(0));
// 	Foret(int _largeur, int _hauteur, std::time_t graine, float proba = 0.60, float _coefFeu = 0.5);
// 	Foret(Foret& other, float proba=0.60);
	/**
	 * Constructeurs de forêt aléatoirement, à partir d'un fichier de sauvegarde
	 * @author Florian
	 * 
	 * @param _largeur nombre de colonnes de la matrice représentant la forêt, sa largeur
	 * @param _hauteur nombre de lignes de la matrice, la hauteur
	 * @param file fichier de sauvegarde d'une foret précédente, contenant les essences et l'emplacement des arbres et leur indice d'essence
	 * @param PB barre de progression Qt, pour afficher l'avancement du chargement
	 */
	Foret(int _largeur, int _hauteur, std::ifstream* file, LoadProgress* PB);
	/**
	 * Constructeurs de forêt aléatoirement, à partir d'une image
	 * @author Florian
	 * 
	 * @param _largeur nombre de colonnes de la matrice représentant la forêt, sa largeur
	 * @param _hauteur nombre de lignes de la matrice, la hauteur
	 * @param matrice matrice d'intensités de couleur verte selon l'emplacement dans l'image
	 */
	Foret(int _largeur, int _hauteur, std::vector< std::vector< int > >* matrice, float coef_brulure);
	/**
	 * On vide également les listes, mêmes si c'est fait automatiquement TODO-SE RENSEIGNER si ca n'entraine pas de "double libération"
-	 * @author Florian
	 */
	virtual ~Foret();
	
	/**
	 * Initialise une matrice avec des cellules (vierge)
	 * @author Ugo and Florian
	 */
	void initEmpty();
	
private:
	/**
	 * Découpe une chaine de caractères en sous-chaine et place chaque élément dans un vecteur de string
	 * 	nécessaire pour l'initialisation des essences d'arbres
	 * @author Ugo
	 * @param str Chaine à découper
	 * @return vecteur des mots séparés par des espaces // TODO TRIM
	 */
	std::vector< std::string >& explode(const std::string& str);
	/**
	 * Choisi une essence pseudo aléatoirement, en prennant en compte les arbre autour d'une position
	 * @author Ugo
	 * 
	 * @param j colonne de la position du futur arbre
	 * @param i ligne de la position du futur arbre
	 * @param distOthers distance à laquelle on doit prendre en compte les arbres avoisinants
	 * @return indice, dans le tableau d'essence, de l'essence "tirée"
	 */
	unsigned essenceRandom(int col, int row, unsigned int distOthers);
	
public:
// Setters
	/**
	 * Définit/redéfinit la taille et le coefficient de brulure d'une foret
	 * @author Florian
	 * @param all définit les paramètres particuliers de la forêt (en dehors du vent)
	 * @deprecated
	 */
	void setValues(int largeur, int hauteur, float coef);
	/**
	 * Définit la direction du vent
	 * @author Florian
	 * @param angle angle du nouveau vent
	 * @param vitesse force du nouveau
	 */
	void setWind(int angle, int vitesse);
	
/* Getters */
	int width()	const { return colonnes; };
	int height()	const { return lignes; };
	int nbEssences(){ return essences.size() ; };
	const Vent* getVent() const { return wind; };
	std::vector< Cellule* >* operator[](int ligne) { return &(matrix[ligne]); };
	
// Initialisations
	/**
	 * Charge des essences dans le tableau d'essences à partir d'un fichier texte
	 * 	Format des lignes : "Nom" "masse volumique (kg/m³)" "diametre moyen dans R" "hauteur moyenne"  "type (0/1)"
	 * @author Ugo
	 * @param fileName nom du fichier qui contient les essences
	 * @return vrai si le fichier a été ouvert, faux sinon
	 */
	bool tryLoadEssences(const std::string& fileName);
	
	/**
	 * Crée une foret à partir d'une matrice de niveau de couleur de vert
	 * @author Florian
	 * @param largeur largeur de la foret à créer
	 * @param hauteur hauteur de la foret à créer
	 * @param matrice de taille largeur x hauteur, d'intensités de vert dans l'image
	 */
	void create(int largeur, int hauteur, std::vector< std::vector< int > >* matrice);
	/**
	 * Initialise une matrice vide puis ajoute des arbre dans la Foret
	 * @author Florian and Ugo
	 * @param probabilite chance qu'a un arbre d'être placé sur chaque case
	 */
	void randomMatrix(float probabilite);
	
	/**
	 * Met la foret à l'état vierge : aucun arbre
	 * @author Florian
	 */
	void clean();
	
	const std::list< Arbre* >* getOnFire() const	{ return &onFire; };
	std::list< Arbre* >* getUprooted()		{ return &uprooted; };
	std::list< Arbre* >* getDelayed()		{ return &delayed; };
	std::list< Arbre* >* getDelayBurned()	{ return &delayBurned; };
	std::list< Arbre* >* getCarbonized()	{ return &carbonized; };
	std::list< Arbre* >* getBurned()			{ return &burned; };
	/**
	 * Retourne les listes d'éléments modfiés
	 * @author Florian
	 *	IMPROVEIT ? Inutilisé, car il faudrait regarder pour chaque élément avec quelle couleur l'afficher au lieu d'une couleur par liste
	 */
	std::list< std::list< Arbre* > >* getChanged();
	
	// Vidage des listes
	void clearUprooted()	{ uprooted.clear(); };
	void clearDelayed()	{ delayed.clear(); };
	void clearDelayBurned()	{ delayBurned.clear(); };
	void clearCarbonized()	{ carbonized.clear(); };
	void clearBurned()	{ burned.clear(); };
	/**
	 * Vide les listes d'éléments modifiés
	 * @author Florian
	 */
	void clearChanged();
	
	
	// Manipulations d'arbre
	/**
	 * "Plante" un arbre à la position donnée dans la matrice
	 * @author Florian et Ugo
	 * @param all position de l'arbre dans la matrice
	 */
	void plantTree(int col, int row);
	/**
	 * "Plante" un arbre à la position donnée dans la matrice
	 * @author Florian et Ugo
	 * @param col abscisse de l'arbre
	 * @param row ordonnée de l'arbre
	 * @param numEss indice de l'essence de l'arbre à utiliser
	 */
	void plantTree(int col, int row, unsigned int numEss, int PdV, float humidite, float coef, int etat);
	
	/**
	 * Coupe ou déracine un arbre
	 * @author Florian
	 * @param ab arbre à supprimer
	 */
	void uproot(Arbre* ab);
	/**
	 * Coupe ou déracine l'arbre à la position donnée
	 * @author Florian
	 * @param all position de l'arbre à supprimer
	 */
	void uproot(int col, int row);
	
	/**
	 * Eteint un arbre en conservant ses points de vie courant
	 * @author Florian
	 * @param ab arbre à éteindre
	 */
	void delay(Arbre* ab, float coef=0.5);
	
	/**
	 * Definit un arbre comme étant en feu
	 * @author Florian
	 * @param ab arbre à allumer
	 */
	void kindle(Arbre* ab);
	/**
	 * Definit une position comme étant en feu, si c'est un arbre
	 * @author Florian
	 * @param all position de l'arbre à allumer
	 */
	void kindle(int col, int row);
	
	/**
	 * Definit un arbre comme étant en cendres
	 * @author Florian
	 * @param ab arbre à mettre en cendres
	 */
	void blast(Arbre* ab);
	// 	void eteindre(int row, int col);
	
	/**
	 * Enflamme un arbre : un autre arbre lui "transmet" du feu
	 * @author Florian
	 * @param ab arbre à enflammer
	 * @param intensite force de transmission du feu
	 */
	void spark(Arbre* ab, int intensite);
	/**
	 * Enflamme une cellule si elle existe, selon sa position dans la matrice
	 * @author Florian
	 * @param row ligne où est la cellule 
	 * @param col colonne où est la cellule
	 * @param intensite force de transmission du feu
	 * @deprecated
	 */
	void spark(int col, int row, int intensite);

	
// Autres méthodes
	/**
	 * Coupe les arbres se trouvant dans un rectangle
	 * @param all positions de 2 points diagonaux du rectangle
	 */
	void cut(int xDep, int yDep, int xArr, int yArr);
	/**
	 * Applique un retardateur sur les arbres se trouvant dans un rectangle
	 * @param all positions de 2 points diagonaux du rectangle
	 */
	void delay(int xDep, int yDep, int xArr, int yArr);
	/**
	 * Retourne les arbres qui sont proches d'une cellule donnée
	 * @author Florian and Ugo
	 * @param col indice de la colonne de la cellule
	 * @param row indice de la ligne de la cellule
	 * @param distance distance sur laquelle s'effectue la recherche de voisins
	 * @return list de pointeurs sur arbres proches
	 */
	std::list< Arbre* > adjacents(int col, int row, int distance) const;
	/**
	 * Retourne les arbres qui sont proches d'une cellule donnée, appelle adjacents(int, int)
	 * @author Florian
	 * @param ab arbre dont on veut connaître les voisins
	 * @param distance distance sur laquelle s'effectue la recherche de voisins
	 * @return list de pointeurs sur arbres
	 */
	std::list< Arbre* > adjacents(const Arbre * ab, int distance) const;
	
	
	/**
	 * Transmet le feu d'un arbre à ses voisins, selon la position de l'arbre
	 * @author Ugo et Florian
	 * 
	 * @param posCol abscisse de l'arbre qui est en feu
	 * @param posRow ordonnée de l'arbre qui est en feu
	 * @param hor force du vent horizontalement
	 * @param vert force du vent verticalement
	 */
	void sparkAdjacentsWind(int posCol, int posRow, int hor, int vert);
	/**
	 * Transmet le feu d'un arbre à ses voisins, selon la position de l'arbre
	 * @author Ugo et Florian
	 * 
	 * @param a arbre qui transmet le feu
	 * @param vent qui influe la transmission
	 */
	void sparkAdjacentsWind(Arbre* a, const Vent* vent);
	
// 	Avancee du temps
	// voir transition avec d'autres parametres
	/**
	 * Applique une transition de l'état t à l'état t+1 d'un arbre
	 * @author Florian et Ugo
	 * @param ab arbre en feu qui doit le transmettre
	 * @deprecated transmission sans vent
	 */
	void transition(Arbre* ab);
	/**
	 * Un arbre brule pendant une unité de temps :
	 * 	il enflamme les voisins en tenant compte de la direction du vent et perd des PV
	 * @author Ugo
	 * 
	 * @param a arbre qui brule
	 * @param vent qui influe la transmission
	 */
	void transitionWind(Arbre* a, const Vent* vent);
	
	/**
	 * Passe de t à t+1 tous les arbres à l'aide de la liste d'arbres en feu
	 * @author Florian et Ugo
	 * @return vrai si il y eu des changements, faux sinon
	 */
	bool NextMove();
	
	
// Persistance des donnees
	/**
	 * Lit les tailles d'une foret dans un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier de sauvegarde de foret
	 * @deprecated taille lues dans fwelcome, pour initialiser les tailles des fenetres
	 */
	void loadSizes(std::ifstream* file);
	/**
	 * Lit le coefficient de brulure d'une foret dans un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier de sauvegarde de foret
	 */
	void loadBurningCoef(std::ifstream* file);
	/**
	 * Lit les essences d'une foret dans un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier de sauvegarde de foret
	 */
	void loadEssences(std::ifstream* file);
	/**
	 * Charge une foret à partir d'un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier de sauvegarde d'une foret
	 * @param PB barre de progression pour suivre le chargement de la foret IMPROVEIT
	 */
	void loadMatrix(std::ifstream* file, LoadProgress* progress);
	/**
	 * Initialise la foret et rempli la matrice d'arbres à partir d"un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier de sauvegarde de foret
	 * @param PB barre de progression pour suivre le chargement de la foret IMPROVEIT
	 * @return vrai si le chargement a reussi ? TODO vérifier
	 */
	bool tryLoad(std::ifstream* file, LoadProgress* progress);
	
	/**
	 * Sauvegarde les tailles et le coefficient de brulure de la foret dans un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier où sauvegarder les données de la forêt
	 */
	void saveProperties(std::ofstream* file);
	/**
	 * Sauvegarde les essences de la foret et leur nombre dans un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier où sauvegarder les données de la forêt
	 */
	void saveEssences(std::ofstream* file);
	/**
	 * Sauvegarde les arbres de la foret dans un fichier de sauvegarde
	 * @author Florian
	 * @param file fichier où sauvegarder les données de la forêt
	 */
	void saveMatrix(std::ofstream* file);
	/**
	 * Sauvegarde une forêt dans un fichier (tailles-essences-arbres)
	 * @author Florian
	 * @param filename chemin où sauvegarder le fichier de sauvegarde
	 * @return vrai si la sauvegarde a réussi ? TODO vérifier
	 */
	bool trySave(std::string filePath = "save_forest");
	
	/**
	 * Sauvegarde la taille et la graine aléatoire d'une forêt
	 * @author Florian
	 * @param file
	 * @return vrai si la sauvegarde a réussi ? TODO vérifier
	 */
	bool trySaveSeed(std::string filePath = "save_forest");
	
// Affichage attributs
	/**
	 * Affiche les essences de la foret sur la sortie standard (debuggage)
	 * @author Florian
	 */
	void showEssences() const;
};

#endif // FORET_H
