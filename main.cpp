#include <QCoreApplication>


#include "terminal.h"

using namespace std;

// Classe terminal temporaire, juste pour affichage en ncurses

int main(int argc, char** argv)
{
//     QCoreApplication app(argc, argv);
//     AutomateFeu foo;
	// mettre dernier para à 1 si on veut mettre 2 fois plus de cellules hztl
		Terminal scr(52, 220, 0.650, 10000, 1);
		
		scr.afficheForet();

		scr.run();
		
		printw("Pierre a un petit kiki");
		scr.end();
		
//     return app.exec();
}
