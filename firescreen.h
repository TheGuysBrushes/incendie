#ifndef FIRESCREEN_H
#define FIRESCREEN_H

#include <QtGui/QMainWindow>
#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

#include "fwelcome.h"
#include "firewidget.h"
/*	TODO :
 * 		- Extraire les paramètres pour initialiser la forêt :
 * 				- AlertDialog au démarrage du programme
 * 		- RAZ de la matrice :
 * 				- AlertDialog puis création nouvelle forêt
 * 
 * 	 DONE : 
 * 		- Gestion du temps continu  
 * 
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
		
	void initialiseParametres(int hauteur, int largeur, float proba, float coef_brulure);
	void initSizes(int _h, int _l);
	
	void majTimer();
		
protected:
	void resizeEvent(QResizeEvent* Qevent);
	
public slots:
	void start_timer(bool);
	void stop_timer(bool);
	void compteur();
	void set_delai(int x );
	void reset();


signals:
	void trans_continue(long );
	void ask_restart();

};

#endif // FIRESCREEN_H
