#include "debug.h"

// #include "vent.h"
#include <QtGui/QApplication>
#include "firescreen.h"
#include "fwelcome.h"

using namespace std;

/**
 * @author Ugo et Florian
 */
int main(int argc, char** argv)
{
	int hauteur;
	int largeur;
	float proba;
	float coef_brulure;
	
	// 	Vent mistralGagnant(2.0, 1.0);
	// 	Vent v1(2.0, 0.0);
	// 	Vent v3= v1;
	// 	
	// 	cout << v3.toString() << "vitesse : "<< v3.getSpeed();
	
	QApplication app(argc, argv);
	
	FireScreen* screen = new FireScreen();
	Fwelcome* fwel = new Fwelcome(screen);
	fwel->show();
	if(fwel->exec() == QDialog::Accepted ){
		hauteur = fwel->get_haut();
		largeur = fwel->get_larg();
		proba = fwel->get_proba();
		coef_brulure = fwel->get_coef();
		screen->initialiseParametres(largeur, hauteur, proba, coef_brulure);
		screen->show();
	}
	
	return app.exec();
}
