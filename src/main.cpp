#include <QtWidgets/QApplication>

#include "debug.h"
#include "Display/firescreen.h"

#include <iostream>

//Objects of QTranslator class provide translations for user-visible text.
#include <QTranslator>
#include <QLocale>

#define STRINGIFY(x) #x
#define VERSION STRINGIFY(1.4.3.1)

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
    QString locale;
    const QString &incendieTrPath = QCoreApplication::applicationDirPath()
            + QLatin1String("/translations");
    clog << "(main)chemin translation : "<< incendieTrPath.toStdString()<< endl;

    ifstream lang;
    lang.open("./translations/incendie.lang", ios::in|ios::binary);
    if (lang.is_open()) {
        // Chargements des parametres dans le fichier
        clog<< "(main)chargement fichier lang"<< endl;
        char language[2];
        lang.read((char *)&(language), 2*sizeof(char));
        cout<< "(main)langue : "<< language[0]<< language[1]<<  endl;

        lang.close();

        locale.append(language[0]);
        locale.append(language[1]);

        //    Tries to load a translation file corresponding to the system language.
        //    No error will occur if the file is not found.
        translator.load(QLatin1String("incendie_") + locale, incendieTrPath);
    } else {
        QStringList uiLanguages;
        uiLanguages = QLocale::system().uiLanguages();
        for (auto it_locale= uiLanguages.cbegin() ; it_locale != uiLanguages.cend(); ++it_locale) {
            locale = QLocale(*it_locale).name();
            if (translator.load(QLatin1String("incendie_") + locale, incendieTrPath)) {
                break;
            } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
                translator.load(QLatin1String("incendie_en"), incendieTrPath);
                break;
            } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
                translator.load(QLatin1String("incendie_en"), incendieTrPath);
                break;
            }
        }
    }
    // Ajoute le fichier de traduction au programme
    app.installTranslator(&translator);
    app.setProperty("incendie_locale", locale);
    clog<< "Locale : "<< locale.toStdString()<< endl;

    FireScreen* screen = new FireScreen(app.tr("Fire") + "_" VERSION);

//    screen->initDefault();
//    screen->show();

    if (screen->tryInitialisation(argc, argv) ) {
        screen->show();
    }else {
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
 * Les tailles des arbres sont fixes, on pourrait utiliser la taille moyenne et l'age de l"arbre
 *  pour calculer et afficher différentes tailles, l'entité arbre serait différente(tronc et feuillage),
 * 	une partie des feuilles des arbres peuvent se superposer à d'autres celles d'autres arbres
 *  et le placement devient plus compliqué.
 * 	Lors des chargements d'images, il faudrait identifier chaque arbre de l'image.
 *
 * Ralentissment de l'exécution lorsqu'il y a beaucoup d'arbres en feu en même temps
 */
