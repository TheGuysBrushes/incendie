#ifndef FIRESCREEN_H
#define FIRESCREEN_H

#include <QtGui/QMainWindow>
#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

#include "fwelcome.h"
#include "firewidget.h"
/*	TODO firescreen:
 * 
 *	 DONE : 
 * - Gestion du temps continu
 *	- Extraire les paramètres pour initialiser la forêt :
 * 	- AlertDialog au démarrage du programme
 * 	
 * - RAZ de la matrice :
 * 	- AlertDialog puis création nouvelle forêt
 */
class FireScreen : public QMainWindow
{
Q_OBJECT
private:
	FireWidget* fwidget;
	QLabel* cpt_lbl;
	QLabel* delai_lbl;
	QPushButton* pause_btn;
	QPushButton* play_btn;
	QPushButton* next_btn;
	
	QTimer* timer;
	int nb_tour;
	long delai;
public:
	FireScreen();
// 	FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure=1.0/*, QWidget* parent = 0, Qt::WindowFlags flags = 0*/);
// 	/*explicit */FireScreen(int hauteur, int largeur, float proba, long int nTemps = 0.60, float coef_brulure=1.0, bool t = 0,
// 									QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~FireScreen();
		
	void initForest(int largeur, int hauteur, float proba, float coef_brulure);
	void initComponents();
	void initSizes(int largeur, int hauteur);
	
	void majTimer();
	bool initialisation();
		
protected:
	void resizeEvent(QResizeEvent* Qevent);
	
public slots:
	void start_timer(bool);
	void stop_timer(bool);
	void nextCompteur();
	void set_delai(int x );
	void reset();

};

#endif // FIRESCREEN_H
