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

#include <cmath>

#include "../Moteur/actions.h"
#include "fwelcome.h"
#include "firewidget.h"
#include "windwidget.h"

/*	firescreen:
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
	WindWidget* windWidget;
	FireWidget* fWidget;
	
	QWidget* menus; // utile pour connaitre sa largeur
	
	QLabel* cpt_lbl;
	QLabel* delai_lbl;

	QPushButton* pause_btn;	// pour pouvoir le griser
	QPushButton* play_btn;	// pour pouvoir le griser
	QPushButton* next_btn;	// pour pouvoir le griser
	QPushButton* cut_btn;	// pour pouvoir le griser
	QPushButton* delay_btn;	// pour pouvoir le griser
	
	QTimer* timer;
	
	int nb_tour;
	long delai;
	
public:
	FireScreen();
// 	FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure=1.0/*, QWidget* parent = 0, Qt::WindowFlags flags = 0*/);
// 	/*explicit */FireScreen(int hauteur, int largeur, float proba, long int nTemps = 0.60, float coef_brulure=1.0, bool t = 0,
// 									QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~FireScreen();
	
	void createForest(int largeur, int hauteur, std::ifstream* file);
	void createForest(std::ifstream* file);
	
	void initForest(Fwelcome* fwel);
	void initMenus(QHBoxLayout* HLayout) /*const*/;
	void initComponents();
	void initSizes(int largeur, int hauteur);
	
	void majCompteur();
	bool initialisation();
		
protected:
	void resizeEvent(QResizeEvent* Qevent);
// 	void mousePressEvent(QMouseEvent* event);
// 	void mouseMoveEvent(QMouseEvent*event);
// 	void mouseReleaseEvent(QMouseEvent* event);
	
public slots:
	/* Timer	*/
	void start_timer();
	void stop_timer();
	void set_delai(int x );
	/*	Boutons	*/
	void nextStep();
	void reset();
	void save() const;
	
	void invertActionRightMouse();
	void updateWind(int angle, int vitesse);
	
	void releaseOrdered();
	
signals:
	void actionSender(int x);

};

#endif // FIRESCREEN_H
