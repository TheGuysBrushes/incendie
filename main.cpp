#include <QCoreApplication>
#include "AutomateFeu.h"

#include "terminal.h"

using namespace std;

// Classe terminal temporaire, juste pour affichage en ncurses
// AutomateFeu inutilisée. Projet en mode KDE-Terminal

int main(int argc, char** argv)
{
//     QCoreApplication app(argc, argv);
//     AutomateFeu foo;
		Terminal scr(50, 40);
		
		scr.afficheForet();

		scr.run();
		
		printw("Pierre a un petit kiki");
		scr.end();
		
//     return app.exec();
}
