#ifndef FIRESCREEN_H
#define FIRESCREEN_H

#include <QtCore/QTimer>
#include <QtCore/QString>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>

#include <cmath>

#include "../Moteur/actions.h"
#include "fwelcome.h"
#include "firewidget.h"
#include "windwidget.h"

class FireScreen : public QMainWindow
{
    Q_OBJECT
private:
    WindWidget* windWidget;
    FireWidget* fWidget;

    QWidget* menus; // utile pour connaitre sa largeur
    QWidget* aboutWidget;

    QLabel* cpt_lbl;
    QLabel* delai_lbl;

    // BARRE de menus
    // Quitter
    QAction* exit;

    //Saves
    QMenu* menuSave;
        QAction* saveDataAction;
        QAction* saveImageAction;
        QAction* saveSeedAction;

    //About
    QAction* about;

    //Langues
    QMenu* menuLang;
        QAction* setLangFRAction;
        QAction* setLangENAction;
        QAction* setLangDEAction;

    // Boutons gestion incendie
    QPushButton* pause_btn;
    QPushButton* play_btn;
    QPushButton* next_btn;
    QSlider* slider;
    QComboBox* actionBox;
    // Boutons gestion sauvegardes et for�t
    QPushButton* reset_btn;
    QPushButton* saveStateBtn;
    QPushButton* saveSeedBtn;
    QPushButton* saveImageBtn;

    Fwelcome* fwel;
    QWidget* mainWidget;
    QHBoxLayout* mainLay;
    QFileDialog* fileSaveDialog;

    int nb_tour;
    long delai;

public:
    /**
     * On initialise les composant graphiques de la classe (foret et menus, boutons), la barre des menus
     * @author Ugo et Florian
     */
    FireScreen();
    // 	FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure=1.0/*, QWidget* parent = 0, Qt::WindowFlags flags = 0*/);
    // 	/*explicit */FireScreen(int hauteur, int largeur, float proba, long int nTemps = 0.60, float coef_brulure=1.0, bool t = 0,
    // 									QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~FireScreen();

    /* Initialisations */
    /**
     * Initialise l'application et crée une nouvelle forêt, en utilisant une fenêtre dédiée (Fwelcome)
     * @author Florian et Ugo
     * @return vrai si l'initialisation a réussi
     */
    bool tryInitialisation();

    /**
     * Initialise l'application et crée une nouvelle forêt, a partir d'une image
     * @author Florian
     * @return vrai si l'initialisation a réussi
     */
    bool tryInitialisation(int argc, char* argv[]);


    /**
     * Definit les tailles maximales de la fenetre
     * @author Florian et Ugo
     * @param largeur nombre de cases de la matrice en largeur
     * @param hauteur nombre de cases en hauteur
     */
    void initSizes(int largeur, int hauteur);
    /**
    * Crée les menus sur la droite de la fenêtre et les place dans un layout
    * @author Ugo (organisation Florian)
    * @param HLayout layout horizontal dans lequel on place les menus
    */
private:
    void initMenus(QHBoxLayout* HLayout);
    /**
     * Crée les menus et place tous les composants dans fenêtre
     * @author Ugo et Florian
     */
    void initComponents();
    /**
     * Crée les connections entre signaux et slots
     */
    void initEvents();
    /**
     * Essai de créer un nouvelle forêt grâce à une fenêtre de paramétrage (Fwelcome)
     * @author Florian
     * @param fwel fenêtre de paramétrage
     * @return faux si l'utilisateur annule ou qu'il y a une erreur
     */
    bool tryInitForest();

    /* Methodes diverses */
    /**
     * Met à jour l'affichage du compteur de tours
     */
    void majCompteur();

    /**
     * Modifie le fichier de langue avec la langue choisie
     *  Le changement de langue prend effet au prochain lancement
     * @return vrai si on a réussi à ouvrir et modifier le fichier
     */
    bool tryChangeLanguage(QLocale lang);

    /**
     * Vérifie que la fenêtre de sauvegarde est initialisée,
     *  si ce n'est pas le cas, elle est initialisée
     */
    void checkInitFileSaveDialog();

protected:
    /**
     * Redéfinition de resizeEvent, utilisée seulement pour le debuggage,
     * pour connaitre la taille de la fenetre et des menus
     * @author Florian
     */
    void resizeEvent(QResizeEvent* Qevent);

public slots:
    /* Timers	*/
    /**
     * Démarre le déroulement continue de l'incendie
     * @author Ugo
     */
    void start_timer();
    /**
     * Arrête le déroulement continue de l'incendie
     * @author Ugo
     */
    void stop_timer();

    /* Sliders */
    /**
     * Définit le nouveau pas utilisée lors de l'avancement continue
     * @author Ugo
     */
    void set_delai(int x );

    /*	Boutons	*/
    /**
     * Avance la progression de l'incendie d'un tour (t+1)
     * @author Florian
     */
    void nextStep();
    /**
     * Une nouvelle fenêtre de création de forêt est ouverte.
     * Si l'utilisateur valide, alors une nouvelle forêt est crée,
     * sinon, l'ancienne forêt est conservée
     * @author Florian et Ugo
     */
    void reset();
    /**
     * Appelle la sauvegarde de la foret dans fWidget
     * @author Ugo et Florian
     */
    void saveData();
    /**
     * Appelle la sauvegarde d'une image de foret dans fWidget
     * @author Ugo et Florian
     */
    void saveImage();
    /**
     * Appelle la sauvegarde d'une graine de foret dans fWidget
     * @author Ugo et Florian
     */
    void saveSeed();

    /**
     * Configure la langue du prochain redémarrage en anglais
     */
    void setLangEN();
    /**
     * Configure la langue du prochain redémarrage en français
     */
    void setLangFR();
    /**
     * Configure la langue du prochain redémarrage en allemand
     */
    void setLangDE();

    /* Autres */
    /**
     * Slot mis en place afin de transmettre l'action sélectionnée à appliquer
     * après une selection sur la matrice. Est connecté au signal émis lors du
     * releaseMouseEvent de fwidget lorsque le clic droit était enfoncé.
     * Voir commentaire Slack pour mise en #define. Pour le moment,
     * le signal 0 correspond à une coupure, le 1 à un retardateur.
     * @author Ugo
     */
    void releaseOrdered();

    /**
     * Slot mis en place afin de faire pop la fenetre d'information
     * sur les développeurs de l'application
     */
    void popAbout();
    // 	void  close(bool);

signals:
    /**
     * Vers firewidget pour effectuer l'action du clic droit
     *
     * @author Ugo
     * @param x
     * TODO Ugo : comment
     */
    void actionSender(int x); //Vers firewidget

};

#endif // FIRESCREEN_H
