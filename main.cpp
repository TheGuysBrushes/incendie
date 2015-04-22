#include "debug.h"

// #include "vent.h"
#include <QtGui/QApplication>
#include "firescreen.h"

using namespace std;

/**
 * @author Ugo et Florian
 */
int main(int argc, char** argv)
{
	int hauteur=	100;
	int largeur=	100;
	float proba=	1;
	long vitesse=	500;
	float coef_brulure= 1.0;

// 	Vent mistralGagnant(2.0, 1.0);
// 	Vent v1(2.0, 0.0);
// 	Vent v3= v1;
// 	
// 	cout << v3.toString() << "vitesse : "<< v3.getSpeed();
	
	QApplication app(argc, argv);
	FireScreen foo(hauteur, largeur, proba, vitesse, coef_brulure);
	foo.show();
	return app.exec();
}
