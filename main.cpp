#include <QtGui/QApplication>

#include "debug.h"
#include "Display/firescreen.h"

using namespace std;

/**
 * @author Ugo et Florian
 */
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	
	FireScreen* screen = new FireScreen();
	if ( screen->initialisation() )
		screen->show();
	
	return app.exec();
}
