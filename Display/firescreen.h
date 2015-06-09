#ifndef FIRESCREEN_H
#define FIRESCREEN_H

#include <QtCore/QTimer>
#include <QtCore/QString>
#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtGui/QComboBox>
#include <QtGui/QFileDialog>

#include <cmath>

#include "../Moteur/actions.h"
#include "fwelcome.h"
#include "firewidget.h"
#include "windwidget.h"

class FireScreen : public QMainWindow
{
Q_OBJECT
private:
	WindWidget* windWidget;
	FireWidget* fWidget;
	
	QWidget* menus; // utile pour connaitre sa largeur
	QWidget* aboutWidget;
	
	QLabel* cpt_lbl;
	QLabel* delai_lbl;

	QPushButton* pause_btn;	// pour pouvoir le griser
	QPushButton* play_btn;	// pour pouvoir le griser
	QPushButton* next_btn;	// pour pouvoir le griser
	QPushButton* cut_btn;	// pour pouvoir le griser
	QPushButton* delay_btn;	// pour pouvoir le griser
	QComboBox* actionBox;
	QTimer* timer;
	
	QFileDialog* fileSaveDialog;
	
	int nb_tour;
	long delai;
	
public:
	/**
	 * On initialise les composant graphiques de la classe (foret et menus, boutons), la barre des menus
	 * @author Ugo et Florian
	 */
	FireScreen();
// 	FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure=1.0/*, QWidget* parent = 0, Qt::WindowFlags flags = 0*/);
// 	/*explicit */FireScreen(int hauteur, int largeur, float proba, long int nTemps = 0.60, float coef_brulure=1.0, bool t = 0,
// 									QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~FireScreen();
	
/* Initialisations */
	/**
	 * Initialise l'application et crée une nouvelle forêt, en utilisant une fenêtre dédiée (Fwelcome)
	 * @author Florian et Ugo
	 */
	bool initialisation();
	
	/**
	 * Definit les tailles maximales de la fenetre
	 * @author Florian et Ugo
	 * @param largeur nombre de cases de la matrice en largeur
	 * @param hauteur nombre de cases en hauteur
	 */
	void initSizes(int largeur, int hauteur);
	/**
	* Crée les menus sur la droite de la fenêtre et les place dans un layout
	* @author Ugo (organisation Florian)
	* @param HLayout layout horizontal dans lequel on place les menus
	*/
private:
	void initMenus(QHBoxLayout* HLayout);
	/**
	 * Crée les menus et place tous les composants dans fenêtre
	 * @author Ugo et Florian
	 */
	void initComponents();
	/**
	 * Essai de créer un nouvelle forêt grâce à une fenêtre de paramétrage (Fwelcome)
	 * @author Florian
	 * @param fwel fenêtre de paramétrage
	 * @return faux si l'utilisateur annule ou qu'il y a une erreur
	 */
	bool initForest(Fwelcome* fwel);
	
/* Methodes diverses */
	void majCompteur();
	
	
		
protected:
	/**
	 * Redéfinition de resizeEvent, utilisée seulement pour le debuggage,
	 * pour connaitre la taille de la fenetre et des menus
	 * @author Florian
	 */
	void resizeEvent(QResizeEvent* Qevent);
	
public slots:
/* Timers	*/
	/**
	 * Démarre le déroulement continue de l'incendie
	 * @author Ugo
	 */
	void start_timer();
	/**
	 * Arrête le déroulement continue de l'incendie
	 * @author Ugo
	 */
	void stop_timer();
	
/* Sliders */
	/**
	 * Définit le nouveau pas utilisée lors de l'avancement continue
	 * @author Ugo
	 */
	void set_delai(int x );
	
/*	Boutons	*/
	/**
	 * Avance la progression de l'incendie d'un tour (t+1)
	 * @author Florian
	 */
	void nextStep();
	/**
	 * Une nouvelle fenêtre de création de forêt est ouverte.
	 * Si l'utilisateur valide, alors une nouvelle forêt est crée,
	 * sinon, l'ancienne forêt est conservée
	 * @author Florian et Ugo
	 */
	void reset();
	/**
	 * Appelle la sauvegarde de la foret dans fWidget
	 * @author Ugo et Florian
	 */
	void saveData();
	/**
	 * Appelle la sauvegarde d'une image de foret dans fWidget
	 * @author Ugo et Florian
	 */
	void saveImage();
	/**
	 * Appelle la sauvegarde d'une graine de foret dans fWidget
	 * @author Ugo et Florian
	 */
	void saveSeed();
	
/* Autres */
	/**
	 * Les paramètres du vent ont été modifiés.
	 * On doit récupérer les valeurs et mettre à jour le vent
	 * @author Ugo
	 * @param angle nouvelle valeur d'angle du vent
	 * @param vitesse nouvelle valeur de vitesse du vent
	 */
	void updateWind(int angle, int vitesse);
	
	/**
	 * Slot mis en place afin de transmettre l'action sélectionnée à appliquer
	 * après une selection sur la matrice. Est connecté au signal émis lors du 
	 * releaseMouseEvent de fwidget lorsque le clic droit était enfoncé.
	 * Voir commentaire Slack pour mise en #define. Pour le moment, 
	 * le signal 0 correspond à une coupure, le 1 à un retardateur.
	 * @author Ugo
	 */
	void releaseOrdered();
		
	/** 
	 * Slot mis en place afin de faire pop la fenetre d'information
	 * sur les développeurs de l'application
	 */
	void popAbout();
// 	void  close(bool);
	
signals:
	/**
	 * Vers firewidget pour effectuer l'action du clic droit
	 * 
	 * @author Ugo
	 * @param x
	 * TODO Ugo : comment
	 */
	void actionSender(int x);

};

#endif // FIRESCREEN_H
