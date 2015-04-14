// #include <QCoreApplication>

#include "terminal.h"

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
	
	int hauteur=	45;
	int largeur=	50;
	float proba=	0.650;
	long vitesse=	15000;
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
	
	Terminal scr(hauteur, largeur, proba, vitesse, small);
	
	scr.afficheForet();

	scr.run();
	
	printw("Pierre a un petit kiki");
	scr.end();
//     return app.exec();
}
