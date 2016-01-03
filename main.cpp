#include <QtWidgets/QApplication>

#include "debug.h"
#include "Display/firescreen.h"
//Objects of QTranslator class provide translations for user-visible text.
#include <QTranslator>
#include <QLocale>

using namespace std;

/**
 * @author Ugo et Florian
 */
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
    cout << "test main" << endl;

//    Creates a QTranslator object without a parent.
    QTranslator translator;
//    Tries to load a translation file corresponding to the system language.
//    No error will occur if the file is not found.
    #if FRENCH
        QString locale= "fr";
    #else
        #if ENGLISH
            QString locale= "en";
        #else
            QString locale = QLocale::system().name();
        #endif
    #endif
    clog<< "Locale : "<< locale.toStdString()<< endl;

    translator.load(QString("incendie_") + locale);
    app.installTranslator(&translator);
	
    FireScreen* screen = new FireScreen();
    if (screen->tryInitialisation(argc, argv) ) {
        screen->show();
	}else {
		
//        FireScreen* screen = new FireScreen();
		if ( screen->tryInitialisation() )
			screen->show();
        else screen->close();
    }
	
	return app.exec();
}

/*
Remarque générales :

 * On pourrait placer les fonctions de chargements et de sauvegarde dans une classe ou un fichier
 * 
 * Les tailles des arbres sont fixes, on pourrait utiliser la taille moyenne et l'age de l"arbre pour calculer
 * 	et afficher différentes tailles, l'entité arbre serait différente(tronc et feuillage),
 * 	une partie des feuilles des arbres peuvent se superposer à d'autres celles d'autres arbres et le placement devient plus compliqué.
 * 	Lors des chargements d'images, il faudrait identifier chaque arbre de l'image.
 * 
 * Ralentissment de l'exécution lorsqu'il y a beaucoup d'arbres en feu en même temps
 */
