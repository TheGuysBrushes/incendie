#include "debug.h"

#include <QtGui/QApplication>

#include "Display/firescreen.h"

using namespace std;

/**
 * @author Ugo et Florian
 */
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	
	FireScreen* screen = new FireScreen();
	cout<< "test avant init screen"<< endl;
	if ( screen->initialisation() )
		screen->show();
	
	return app.exec();
}
