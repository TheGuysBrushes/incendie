#include <QApplication>

//#include "terminal.h"
#include "vent.h"
#include "qwindow.h"

using namespace std;

// Classe terminal temporaire, juste pour affichage en ncurses

/**
 * @author Ugo et Florian
 */
int main(int argc, char** argv)
{
	//     QCoreApplication app(argc, argv);
	//     AutomateFeu foo;
	// mettre dernier para à 1 si on veut mettre 2 fois plus de cellules hztl (1600/900_Konsole: maxL:110/220 H: 52)
	
	int hauteur=	30;
	int largeur=	50;
	float proba=	0.55;
	long vitesse=	20000;
	bool small=		false;
	
	if (argc>1)
		hauteur= atoi(argv[1]);
	if (argc>2)
		largeur= atoi(argv[2]);
	if (argc>3)
		proba= atof(argv[3]);
	if (argc>4)
		vitesse= atoi(argv[4]);
	if (argc>5)
		small= atoi(argv[5]);
	
	QApplication app(argc, argv);
	Qwindow foo(hauteur, largeur, proba, vitesse);
	foo.show();
	return app.exec();
	
	/*
	 *	Terminal scr(hauteur, largeur, proba, vitesse, small);
	 *	
	 *	scr.afficheForet();
	 * 
	 *	scr.run();
	 *	
	 *	printw("Pierre a un petit kiki");
	 *	scr.end();
	 *	
	 *	Vent mistralGagnant(2.0, 1.0);
	 *	Vent v1(2.0, 0.0);
	 *	Vent v3= v1;
	 *	
	 *	cout << v3.toString() << "vitesse : "<< v3.getSpeed();
	 */
	//     return app.exec();
}
