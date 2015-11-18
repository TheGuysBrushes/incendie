#ifndef FWELCOME_H
#define FWELCOME_H

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
#include <QtGui/QProgressBar>
#include <QtGui/QGridLayout>
#include <QtGui/QImage>
#include <QtGui/QFileDialog>
// Pour les valeurs max des SpinBox
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>

#include <iostream>
#include <fstream>
#include <ctime>

#include "../debug.h"

enum DialogCode{Rejected, Accepted, Load, Restore, RestoreSeed};

/**
 * Classe représentant le panneau de dialogue
 * pour la sélection des paramètres de l'automate
 * @author Ugo
 */
class Fwelcome : public QDialog
{
Q_OBJECT
private:
	QGridLayout* gridLayButtons;
	QSpinBox* haut_spin;
	QSpinBox* larg_spin;
	QLabel* p_value;
	QLabel* c_value;
	
	QPushButton* valid_btn;
	QPushButton* cancel_btn;
	QPushButton* loadFromImgBtn;
	QPushButton* restoreBtn;
	QPushButton* seedBtn;
	
	QImage* pictureForest;
	
	QFileDialog* fileDialog;
	QDir picturesBrowserLocation;
	
	float proba;
	float burningCoef;
	std::time_t seed;
	
	std::ifstream* file;

public:
/* Constructeur et destructeur */
	/** Instancie les attributs, dont ceux graphiques **/
	Fwelcome(QWidget* parent);
	/** Instancie les attributs, dont ceux graphiques et les valeurs initiales de taille **/
	Fwelcome(QWidget* parent, int _largeur, int _hauteur);
	virtual ~Fwelcome();

/* Gestion graphique */
	/**
	 * Instancie les composants graphiques de la classe
	 * @author Ugo et Florian
	 */
	void createComponents();
	/**
	 * Initialise tous les composants graphiques
	 * @author Ugo et Florian
	 */
	void initComponents();
	/**
	 * Ajoute le bouton cancel à l'interface de création de forêt
	 * @author Florian et Ugo
	 */
	void addCancel() const;
	
	/* Setters */
public slots:
	/**
	 * Met à jour la valeur de la probabilité qu'une cellule soit un arbre
	 * et affiche la valeur courante.
	 * @param int Probabilité qu'une cellule devienne un arbre.
	 * @author Ugo
	 */
	void setProba(int x);
	/**
	 * Met à jour la valeur du coefficient de combustion de l'incendie
	 * et affiche la valeur courante
	 * @param int Coefficient de combustion
	 * @author Ugo
	 */
	void setCoef(int x);
	
public:
/* Getters */
	int getHaut() const	{ return haut_spin->value(); };
	int getLarg() const	{ return larg_spin->value(); };
	float getProba() const	{ return proba; };
	float getCoef() const	{ return burningCoef; };
	float getSeed() const	{ return seed; };
	
	std::ifstream* getFile() { return file; };
	QImage* getImage() { return pictureForest; };
	
/* Chargements */
	/**
	* Ouvre un fichier à partir de son chemin, enregistré dans fwelcome
	* @author Florian
	* @param filename chemin du fichier à ouvrir
	*/
	void openFile(QString filename);
	/**
	 * Lit les tailles d'une forêt dans et les assigne aux spinboxs
	 * @author Florian
	 */
	void loadSizes();
	
	/**
	 * Restaure une forêt stockée dans un fichier
	 * @author Florian
	 * @param filename chemin du fichier de sauvegarde
	 */
	void restore(QString filename);
	/**
	 * Crée un forêt à partir d'une image
	 * @author Florian
	 * @param filename chemin de l'image
	 */
	void loadFromImg(QString filename);
	/**
	 * Utilise une graine aléatoire stockée dans un fichier pour créer une forêt
	 * @author Florian
	 * @param filename chemin du fichier contenant la graine
	 */
	void loadSeed(QString filename);

public slots:
	// Slots d'ouverture FileDialog pour chargement
	
	/**
	 * Slot déclenché lors du clic sur le bouton d'importation d'image.
	 * Ouvre une fenetre de selection d'un fichier image et appelle
	 * la fonction de chargement d'image.
	 * @author Ugo
	 */
	void popImageDIalog();
	/**
	 * Slot déclenché lors du clic sur le bouton de chargement d'une sauvegarde.
	 * Ouvre une fenetre de sélection d'un fichier charge la forêt de ce fichier.
	 * @author Ugo
	 */
	void popSaveDialog();
	/**
	 * Slot déclenché lors du clic sur le bouton de chargement d'une graine.
	 * Ouvre une fenetre de sélection d'un fichier et crée une forêt grâce à ce fichier.
	 * @author Ugo et Florian
	 */
	void popSeedDialog();
};

#endif // FWELCOME_H
