#ifndef FIREWIDGET_H
#define FIREWIDGET_H

#include <QtCore/QByteArray>
#include <QtCore/QPoint>

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QRubberBand>
#include <QtGui/QProgressBar>
#include <QtGui/qevent.h>	// IMPROVEIT pas d'equivalent sans .h ?

#include <vector>
#include <fstream>

// Macros
#include "../Moteur/actions.h"
#include "../debug.h"

#include "../Moteur/foret.h"
#include "fwelcome.h"
#include "loadwindow.h"

/**
 * Widget d'affichage de l'automate cellulaire. Ce composant
 * permet de suivre graphiquement l'évolution de l'automate. 
 * Il utilise un système de buffering et s'appuie sur la classe
 * Foret du moteur.
 * @author Ugo et Florian
 */
class FireWidget : public QWidget {
Q_OBJECT
private:
	Foret* forest;
	QImage* buffer;
	QColor* color;
	QPainter* bufferPainter;
	QImage* pictureForest;	// Image de fond
	
	QPoint origin;
	QRubberBand* rubber;
	// Points de départ et d'arrivée de la zone de selection (redondance pour origine mais normal pour l'instant)
	QPoint depart;
	QPoint arrivee;
	
	qreal tailleCell;
	long temps;
	bool running;
	
private:
	void initRubber(QMouseEvent* event);
	
public:
/* Constructeur et desctructeur */
	/**
	 * Initialise les différents pointeurs et fixe la taille minimale du widget
	 * @author Ugo et Florian
	 * 
	 * @param int _largeur : nombre de colonnes de la matrice 
	 * @param int _hauteur : nombre de lignes de la matrice
	 * @param float _proba : probabilité qu'une cellule deviennent un arbre
	 * @param float _coef : coefficient de combustion de l'incendie
	 */
	FireWidget();
	virtual ~FireWidget();

/* Initialisations */
	/**
	 * Fonction de création d'une foret ALEATOIRE lors de la (ré)initialisation
	 * @author Florian et Ugo
	 * @param all identiques au constructeur de Foret aléatoire
	 */
	void initialise(int largeur, int hauteur, float proba = 0.60, float coef_brulure=1.0, std::time_t graine=std::time(0));
	/**
	 * Fonction de création d'une foret A PARTIR D'UNE SAUVEGARDE,
	 * dans un fichier ouvert dont les tailles ont déjà été lues, lors de la (ré)initialisation
	 * @author Florian et Ugo
	 * 
	 * @param largeur de la nouvelle forêt
	 * @param hauteur de la nouvelle forêt
	 * @param file fichier binaire, ouvert, contenant la sauvegarde de la forêt (essences-arbres)
	 * @return vrai si le fichier est ouvert
	 */
	bool initialise(int largeur, int hauteur, std::ifstream* file);
	/**
	 * Fonction de création d'une foret ALEATOIRE lors de la (ré)initialisation
	 * @author Florian et Ugo
	 * 
	 * @param largeur de la nouvelle forêt
	 * @param hauteur de la nouvelle forêt
	 * @param imageForet image d'une foret, pour l'instant une image converie du jpg au tif
	 * @return vrai si l'image est chargée
	 */
	bool initialise(int largeur, int hauteur, QImage* imageForet, float coef_brulure);

private:
	LoadWindow* createProgressWindow() const;
	/**
	 * Fonction permettant de fixer la couleur à utiliser pour dessiner un arbre
	 * @param int indice de l'essence de l'arbre
	 * @author Florian et Ugo
	 * IMPROVEIT plus de nuances de couleurs ?, une couleur par cellule : on utiliserai une methode differente de "peinture" ? 
	 */
	void setColor(int colorIndice);
	
public:
/* Setters */
	void setWind(float angle, float vitesse)	{ forest->setWind(angle, vitesse); };
	void razRubber() 	{ rubber= NULL; };
/* Gestion Foret */
	/**
	 * Méthodes de destruction de la forêt pour gérer la 
	 * ré-initialisation de la matrice
	 * @author Florian
	 */
	void delForest();
	/**
	 * Crée une nouvelle image de fond vierge
	 * @author Florian
	 */
	void delPicture();
	
	/* Getters */
	int getTailleCell() const { return tailleCell; };
	Foret* getForet() const { return forest; };
	
/* Gestion Sauvegardes */
	/**
	 * Fonction à commenter par son auteur :p
	 * @author Florian
	 * @param imageForet image de la forêt à charger
	 * @param coef_brulure coefficient de brulure de la nouvelle forêt
	 * @param others taille de l'image/forêt
	 */
	void loadFromPicture(int largeurImage, int hauteurImage, QImage* imageForet, float coef_brulure);
	/**
	 * Sauvegarde la foret dans un fichier de sauvegarde
	 * @author Florian
	 * @param filePath chemin du nouveau fichier de sauvegarde
	 */
	void saveForest(std::string& filePath) const;
	/**
	 * Sauvegarde la graine aléatoire et les caractéristiques de la forêt dans un fichier de sauvegarde
	 * @author Florian
	 * @param filePath chemin du nouveau fichier de sauvegarde
	 */
	void saveSeed(std::string& filePath) const;
	/**
	 * Sauvegarde la forêt sous forme d'image
	 * @author Florian
	 * @param filePath chemin du nouveau fichier de sauvegarde
	 */
	bool saveImage(QString filename) const;
	
	/* Modifications de l'état des arbres */
	/**
	 * Eteint un arbre à une position donnée
	 * @author Florian
	 * @param int/int indices de la colonne et de la ligne de l'arbre à éteindre
	 * @return vrai si il y avait un arbre en feu
	 */
// 	bool eteindreFeu(int colonne, int ligne);
	/**
	 * Allume un feu sur un arbre "vivant"
	 * @author Florian
	 * @param colonne de l'arbre à enflammer
	 * @param ligne de l'arbre à enflammer
	 * @return vrai si il y avait un arbre enflammable
	 */
	bool allumerFeu(int colonne, int ligne);
	/**
	 * Opere une combustion complete sur un arbre en feu (IMPROVEIT faire sur les arbres non en feu ?)
	 * @author Florian
	 * @param ligne
	 * @param colonne ligne et colonne de l'arbre à bruler totalement
	 * @return vrai si il y avait un arbre en feu
	 */
	bool finirFeu(int colonne, int ligne);

/* Affichage */
	/**
	 * Imprime une cellule à une position donnée, utilise la couleur courante
	 * @author Florian
	 * @param int col,row indices de la colonne et de la ligne de la cellules
	 */
	void drawCell(int colonne, int ligne);
	/**
	 * Imprime un arbre selon sa position, utilise la couleur courante.
	 * @author Florian
	 * @param ab arbre à dessiner
	 * @deprecated
	 */
	void drawTree(const Arbre* ab);
	/**
	 * Dessine l'ensemble des arbres de la liste passée en paramètre
	 * @param arbres liste des arbres à dessiner
	 * @author Florian et Ugo (commentaires :p )
	 */
	void drawList(std::list< Arbre* >* arbres);
	/**
	 * Dessine les arbres et cellules vides dans le buffer
	 * @author Ugo et Florian
	 */
	void drawForest();
	/**
	 * Fonction à commenter par son auteur :p
	 * @author Florian
	 */
	bool drawPicture();
	/**
	 * Dessine la forêt avec une image en dessous
	 * 	Les arbres ne sont donc pas dessinés
	 * @author Florian
	 */
	bool drawPictureForest();
	/**
	 * Redessine les arbres qui ont changés d'état, sur l'ancienne matrice
	 * On réutilise les cellules non susceptibles d'avoir été modifiées
	 * @author Florian and Ugo
	 */
	// IMPROVEIT faire une fonction qui prend une couleur et une liste d'arbres, qui "imprime" les arbres avec cette couleur ?	
	void drawChanged();
	/**
	 * Vide le buffer et rafraichit l'affichage
	 * @author Florian et Ugo
	 */
	void redraw();

	/** @deprecated */
// 	void reset(int _larg, int _haut, float proba, float coef);

protected:
	/* Events */
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
public slots:
	/**
	 * Passe de l'etat t à t+1 la foret
	 * @author Florian
	 * @return vrai si la foret a ete modifiée
	 */
	bool next();
	/**
	 * Execute l'action choisie sur une zone, suite à releaseMouseEvent.
	 * 0 correspond à une coupure, 1 à un retardateur
	 * @author Ugo
	 */
	void actionReceived(int x);
	
signals:
	void releaseSignal(); // Vers firescreen, récupere l'action sélectionnée dans les menus
	
};

#endif // FIREWIDGET_H
