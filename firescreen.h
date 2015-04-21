#ifndef FIRESCREEN_H
#define FIRESCREEN_H

#include <QtGui/QMainWindow>

#include "firewidget.h"
/*	TODO :
 * 		- Extraire les paramètres pour initialiser la forêt :
 * 				- AlertDialog au démarrage du programme
 * 		- RAZ de la matrice :
 * 				- AlertDialog puis création nouvelle forêt
 * 	
 * 		- Gestion du temps continu  
 * 
 */
class FireScreen : public QMainWindow
{
Q_OBJECT
private:
	FireWidget* fwidget;
	bool play;
	long delai;
public:
	FireScreen();
	virtual ~FireScreen();
	
public slots:
	void active_trans(bool x);
	void stop_trans(bool x);
signals:
	void trans_continue(long );
};

#endif // FIRESCREEN_H
