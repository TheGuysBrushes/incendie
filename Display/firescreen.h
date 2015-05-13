#ifndef FIRESCREEN_H
#define FIRESCREEN_H

#include <QtGui/QMainWindow>
#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>

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
	
	QWidget* menus;
	
	QLabel* cpt_lbl;
	QLabel* delai_lbl;
	QPushButton* pause_btn;
	QPushButton* play_btn;
	QPushButton* next_btn;
	QPushButton* cut_btn;
	QPushButton* delay_btn;
	
	QTimer* timer;
	int nb_tour;
	long delai;
public:
	FireScreen();
// 	FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure=1.0/*, QWidget* parent = 0, Qt::WindowFlags flags = 0*/);
// 	/*explicit */FireScreen(int hauteur, int largeur, float proba, long int nTemps = 0.60, float coef_brulure=1.0, bool t = 0,
// 									QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~FireScreen();
		
	void initForest(const Fwelcome* fwel);
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
	void start_timer();
	void stop_timer();
	void nextStep();
	void set_delai(int x );
	void reset();
	void invertBtn(bool);
	void updateWind(int angle, int vitesse);
	void releaseOrdered();
	void save() const;
	
signals:
	void actionSender(int x);

};

#endif // FIRESCREEN_H
