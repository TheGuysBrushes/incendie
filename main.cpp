#include <QCoreApplication>


#include "terminal.h"

using namespace std;

// Classe terminal temporaire, juste pour affichage en ncurses

int main(int argc, char** argv)
{
//     QCoreApplication app(argc, argv);
//     AutomateFeu foo;
		Terminal scr(51, 110, 0.650, 200);
		
		scr.afficheForet();

		scr.run();
		
		printw("Pierre a un petit kiki");
		scr.end();
		
//     return app.exec();
}
