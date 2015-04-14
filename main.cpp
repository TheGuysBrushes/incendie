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
		Terminal scr(35, 60, 0.600, 50000, 0);
		
		scr.afficheForet();

		scr.run();
		
		printw("Pierre a un petit kiki");
		scr.end();
		
//     return app.exec();
}
