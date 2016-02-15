#include "firescreen.h"

// Composants Qt qui ne sont pas des attributs de classe
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtGui/QImage>

using namespace std;

FireScreen::FireScreen(): QMainWindow()
{
    // Elements de la barre de menus
    exit= menuBar()->addAction(tr("Quit"));

    menuSave= menuBar()->addMenu( tr("Others") );
        saveDataAction = menuSave->addAction( tr("complete Forest") );
        saveImageAction = menuSave->addAction( tr("to Image") );
        saveSeedAction = menuSave->addAction( tr("random seed") );
        //: Presentation menu for the app
        about = menuSave->addAction(tr("About"));

    menuLang= menuSave->addMenu( tr("Languages") );
        setLangENAction= menuLang->addAction( "English");
        setLangFRAction= menuLang->addAction( "Français");
        setLangDEAction= menuLang->addAction( "Deutsch");

    // Composants Qt de la classe
    fWidget= new FireWidget();
    windWidget = new WindWidget();
    menus = new QWidget();
    timer = new QTimer();

    // COMBOBOX des différentes actions
    actionBox = new QComboBox(this);
    actionBox->addItem("");

    next_btn = new QPushButton( tr("Next") );
    play_btn = new QPushButton( tr("Play") );
    pause_btn = new QPushButton( tr("Pause") );
        pause_btn->setDisabled(true);

    // Menu déroulant d'actions du clic droit
    actionBox->addItem( tr("Cutting") );
    actionBox->addItem( tr("Retardator") );

    fwel = new Fwelcome(this);
    fileSaveDialog = NULL;

    cpt_lbl = new QLabel();
    delai_lbl = new QLabel();
    aboutWidget = NULL;
}

FireScreen::~FireScreen()
{
    delete windWidget;
    delete fWidget;

    delete menus;
    if (aboutWidget != NULL) delete aboutWidget;

    delete cpt_lbl;
    delete delai_lbl;

    // BARRE de menus
    delete exit;

    delete menuSave;
        delete saveDataAction;
        delete saveImageAction;
        delete saveSeedAction;
    delete about;
    delete menuLang;
        delete setLangENAction;
        delete setLangFRAction;
        delete setLangDEAction;

    delete pause_btn;
    delete play_btn;
    delete next_btn;
    delete slider;
    delete actionBox;

    delete  reset_btn;
    delete  saveStateBtn;
    delete  saveSeedBtn;
    delete  saveImageBtn;

    delete timer;

    delete fwel;
    delete mainWidget;
    delete mainLay;

    if (fileSaveDialog != NULL) delete fileSaveDialog;
}


bool FireScreen::tryInitialisation()
{
    initComponents();
    initEvents();

    if ( tryInitForest() )	{
        return true;
    }
    else {
        return false;
    }
}

/// A CORRIGER (pop est show(), mais jamais hide())
bool FireScreen::tryInitialisation(int argc, char* argv[])
{
    if (argc != 2)
        return false;

    initComponents();
    initEvents();

    QString ch=argv[1];
    //ch+="\\";
    //ch+=argv[1];
    QWidget* pop= new QWidget();
    QVBoxLayout* VLay= new QVBoxLayout(pop);
    QLabel* arg= new QLabel(ch);
    VLay->addWidget(arg);
    pop->show();

    fwel->loadFromImg(ch);

    // fWidget->delPicture();
    nb_tour = 0;
    int largeur= fwel->getLarg();
    int hauteur= fwel->getHaut();

    // Crée une forêt à partir de l'analyse d'une photo (de forêt de préférence, btw)
    // WARNING coef par défaut, soit 0.5 lors ouverture depuis image
    if (fWidget->tryInitialise(largeur,hauteur, fwel->getImage(), 0.5))	{
        //delete fwel;
        fwel->hide();
        windWidget->initValues(30, 80);
        return true;
    }
    else {
        //delete fwel;
        fwel->hide();
        return false;
    }

//    delete pop;
//    delete VLay;
//    delete arg;
}

void FireScreen::initSizes(int largeur, int hauteur)
{
    int largeurMenu= menus->sizeHint().width();

    // Maximums
    // les 25 pixels car il doit y avoir des marges (observé 14 puis 24 ??)
    int freePixWidth= QApplication::desktop()->screenGeometry().width() -25;
    // 45 pixel à cause des marges et menu (observé 43)
    int freePixHeight= QApplication::desktop()->screenGeometry().height()-45 ;

    int maxCellWidth= (freePixWidth - largeurMenu) / largeur;
    int maxCellHeight= freePixHeight/hauteur;
    int tCellMax= std::min(maxCellWidth, maxCellHeight);

    setMaximumWidth( QApplication::desktop()->screenGeometry().width() );
    setMaximumHeight( QApplication::desktop()->screenGeometry().height() );

    // resize(lay->sizeHint().height()+250 +10, lay->sizeHint().height() +menuBar()->sizeHint().height());
    // TODO mettre une largeur de base minimum, à partir de la hauteur du menu droite
        // (calculer la taille d'une cellule si la hauteur de la fenetre est la hauteur du menu)
    resize( (tCellMax+1)/2 * largeur + largeurMenu +25, (tCellMax + 1)/2 * hauteur +45 );

    /// ATTENTION les valeurs max sont redéfinies dans ce debug,
    ///  il faut transposer les valeurs correctes au dessus (pour "performances")
#if DEBUG_DIMENSION
    std::cout<< "taille cell max : "<< tCellMax<< std::endl;

    int maxLarg= tCellMax * largeur + largeurMenu +25;
    setMaximumWidth( maxLarg);
    std::cout<< "larg max window "<< maxLarg<< "px sur "<< freePixWidth	<<
                " dont "<< largeurMenu<< " menus, dispos"<<std::endl;

    int maxHaut= tCellMax * hauteur +45;
    setMaximumHeight(maxHaut);
    std::cout<< "haut max window "<< maxHaut<<	"px sur "<< freePixHeight	<<	" dispos"<<std::endl;
    std::cout<< std::endl;
#endif
}

void FireScreen::initMenus(QHBoxLayout* HLayout)
{
    HLayout->addWidget(menus);
    // Propriétés utiles?
        menus->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QVBoxLayout* vert_lay1 = new QVBoxLayout(menus);
    QWidget* ww1 = new QWidget();
    QWidget* ww2 = new QWidget();
    QWidget* ww3 = new QWidget();
    QWidget* ww4 = new QWidget();

    QGridLayout* grid_lay1 = new QGridLayout(ww1);
    QGridLayout* grid_lay2 = new QGridLayout(ww2);
    QGridLayout* grid_lay3 = new QGridLayout(ww4);

    QWidget* www = new QWidget(ww2);

    QHBoxLayout* h_lay1 = new QHBoxLayout(www);
    QHBoxLayout* h_lay2 = new QHBoxLayout(ww3);

    //ELEMENTS

    // Element du panneau de direction de l'automate
    QLabel* titre = new QLabel(tr("Cellular automaton") );
    QLabel* info_vent= new QLabel( tr("Wind's settings :") );

    // Boutons
    QLabel* trans_p2p = new QLabel( tr("Step-to-step transmission : ") );

    QLabel* save_text = new QLabel( tr("Saves and creation: ") );
    saveStateBtn = new QPushButton( tr("Current state") );
    saveSeedBtn = new QPushButton( tr("Seed's forest") );
    saveImageBtn  = new QPushButton( tr("As image") );
    reset_btn = new QPushButton( tr("Reset ! Be careful") );

    // Compteur de tours et Slider
        //: Sous-titre
    QLabel* trans_con = new QLabel( tr("Continuous transmission : ") );
        //: Compteur de tours
    QLabel* tour_lbl = new QLabel( tr("Number of turns :") );

    int interTempsInit= 200;

    // 	majCompteur(); REMOVEIT ?

    // Ajouter la scrollbar horizontale
    set_delai(interTempsInit);

    slider = new QSlider(Qt::Horizontal);
        slider->setMinimum(10);
        slider->setMaximum(500);
        slider->setValue(interTempsInit);	// position initiale du slider


// Ajout des éléments dans les conteneurs
    grid_lay1->addWidget(next_btn,0,0);

    grid_lay2->addWidget(play_btn,0,0);
    grid_lay2->addWidget(pause_btn,0,1);
    grid_lay2->addWidget(slider,1,0);

    grid_lay3->addWidget(saveStateBtn,0,0);
    grid_lay3->addWidget(saveSeedBtn, 0, 1);
    grid_lay3->addWidget(saveImageBtn,0,2);
    grid_lay3->addWidget(reset_btn,1,0,1,0);

    h_lay1->addWidget(tour_lbl);
    h_lay1->addWidget(cpt_lbl);

    QLabel* actionLabel = new QLabel( tr("Action of right clic :") );

    h_lay2->addWidget(actionLabel);
    h_lay2->addWidget(actionBox);
    vert_lay1->addWidget(titre);
    vert_lay1->addWidget(info_vent);
    vert_lay1->addWidget(windWidget);
    vert_lay1->addWidget(trans_p2p);
    vert_lay1->addWidget(ww1);
    vert_lay1->addWidget(trans_con);
    vert_lay1->addWidget(ww2);
    vert_lay1->addWidget(www);
    vert_lay1->addWidget(ww3);
    vert_lay1->addWidget(save_text);
    vert_lay1->addWidget(ww4);

    vert_lay1->setAlignment(titre,Qt::AlignHCenter);

/*** 	DEFINITTION DU STYLE DES ELEMENTS	***/
    // Touches d'améliorations visuelles et d'initialisation de propriétés
    titre->setStyleSheet("color : darkblue; font : bold italic 20px;");
    trans_con->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
    trans_p2p->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
    save_text->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
    info_vent->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
    tour_lbl->setStyleSheet("QLabel {  color : darkblue; }");
    cpt_lbl->setStyleSheet("QLabel { color : darkblue; }");
    delai_lbl->setStyleSheet("QLabel { color : darkblue; }");
    actionLabel->setStyleSheet("QLabel { color : darkblue; }");
}


void FireScreen::initComponents(/*, QWidget* parent, Qt::WindowFlags flags*/)
{
    /*** 	BOUTONS ET INTERFACE		***/

    // CONTENEURS
    // Conteneur général
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    // Sous-conteneurs
    mainLay= new QHBoxLayout(mainWidget);

    // PLACEMENT DES ELEMENTS
    // Partie gauche
    mainLay->addWidget(fWidget);
    mainLay->setStretchFactor(fWidget, 1);
    mainLay->minimumHeightForWidth(1);
    // Partie droite, menus
    initMenus(mainLay);

    // 	Definition de la taille selon les éléments
    setMinimumHeight(mainLay->sizeHint().height() + menuBar()->sizeHint().height());
}

void FireScreen::initEvents()
{
    /***  SIGNAUX BARRE DE MENUS   ***/
    // Quitter
    connect(exit, 	SIGNAL(triggered()), SLOT(close()) );
    // Saves
    connect(saveDataAction, 	SIGNAL(triggered()), SLOT(saveData()) );
    connect(saveImageAction, 	SIGNAL(triggered()), SLOT(saveImage()) );
    connect(saveSeedAction, 	SIGNAL(triggered()), SLOT(saveSeed()) );
    // Langues
    connect(setLangENAction, 	SIGNAL(triggered()), SLOT(setLangEN()) );
    connect(setLangFRAction, 	SIGNAL(triggered()), SLOT(setLangFR()) );
    connect(setLangDEAction, 	SIGNAL(triggered()), SLOT(setLangDE()) );
    // A propos
    connect(about, SIGNAL(triggered()), SLOT(popAbout()) );

    /***   SIGNAUX MENUS DE DROITE  ***/
    // Vent
    connect(windWidget,	SIGNAL(wind_changed(int, int)),	fWidget, SLOT(setWind(int, int)) );

    /* Déroulement*/
    connect(next_btn,	SIGNAL(clicked()), 	this,	SLOT( nextStep()) );
    connect(play_btn,	SIGNAL(clicked()), 	this,	SLOT( start_timer()) );
    connect(pause_btn,	SIGNAL(clicked()),	this,	SLOT( stop_timer()) );

    /* Slider */
    connect(slider,	SIGNAL(valueChanged(int)),		this, SLOT( set_delai(int)) );

    // Autres signaux déroulement
    connect(timer,	SIGNAL(timeout()), this,	SLOT( nextStep()) );

    // Coupure et retardateur
    connect(fWidget,	SIGNAL(releaseSignal()),	this, SLOT(releaseOrdered()));
    connect(this,	SIGNAL(actionSender(int)),		fWidget, SLOT(actionReceived(int)));
    /* Gestion foret */
    connect(reset_btn,	SIGNAL(clicked()), 	this,	SLOT( reset()) );
    connect(saveStateBtn,	SIGNAL(clicked()),	this, SLOT( saveData()) );
    connect(saveSeedBtn, SIGNAL(clicked()), this, SLOT( saveSeed()) );
    connect(saveImageBtn, SIGNAL (clicked()), this,  SLOT(saveImage()));
}

bool FireScreen::tryInitForest()
{
    fwel->show();
    int resExec= fwel->exec();
    fwel->hide();
    if (resExec>0){

        fWidget->delPicture();
        nb_tour = 0;
        int largeur= fwel->getLarg();
        int hauteur= fwel->getHaut();

        if(  resExec==Load ){
            // 		  Crée une forêt à partir de l'analyse d'une photo (de forêt de préférence, btw)
            fWidget->tryInitialise(largeur,hauteur, fwel->getImage(), fwel->getCoef());
            windWidget->initValues(30, 80);
        }
        else if(  resExec==Restore ){
            // 		  Restaure une forêt à partir d'une fichier
            fWidget->tryInitialise(largeur,hauteur, fwel->getFile());
            windWidget->initValues(30, 80);
        }
        else if( resExec==RestoreSeed ){
            // 		  réutilise la graine aléatoire déjà existante
            fWidget->initialise(largeur,hauteur,
                                fwel->getProba(), fwel->getCoef(), fwel->getSeed() );
            windWidget->initValues(30, 80);
        }
        else if( resExec==Accepted ){
            // 		  Crée une nouvelle forêt aléatoirement, a partr d'une graine aléatoire
            fWidget->initialise(largeur,hauteur,
                                fwel->getProba(), fwel->getCoef()	);
            windWidget->initValues(30, 80);
        }
        else {
            cerr<< "Mauvais code de retour de la fenetre de paramétrage"<< endl;
            return false;
        }

        majCompteur();
        initSizes(largeur, hauteur);

        return true;
    }
    else
        return false;
}


// ##############################
/***		Autres methodes		***/
// ##############################

/**
 * Met à l'affichage du timer, utilise nb_tour
 * @author Florian
 */
void FireScreen::majCompteur()
{
    cpt_lbl->setText(QString::number(nb_tour));
}

bool FireScreen::tryChangeLanguage(QLocale lang)
{
    string chemin= "./translations/incendie.lang";
    ofstream* file= new ofstream(chemin.c_str(), std::ios::out|std::ios::binary);

    if (!file->is_open()){
        cerr<< "Echec ouverture fichier de langue"<< chemin<< endl;
        delete file;

        return false;
    }
    else {
        char word[2];
        word[0]= lang.name().at(0).toLatin1();
        word[1]= lang.name().at(1).toLatin1();

        //Ecriture dans le fichier
        file->write( (char *)&word[0], sizeof(char));
        file->write( (char *)&word[1], sizeof(char));

        clog<< "Sauvegarde de la langue dans "<< chemin<< " réussie"<< endl;

        file->close();
        delete file;

        return true;
    }
}

// #############
// 	Events
// #############
void FireScreen::resizeEvent(QResizeEvent* Qevent)
{
    QWidget::resizeEvent(Qevent);

#if DEBUG_DIMENSION
    std::cout << "WWindow: "<< Qevent->size().width()<< " HWindow: "<< Qevent->size().height()<< std::endl;
    std::cout << "WMenu: "<< menus->sizeHint().width()<< " HMenu: "<<menus->sizeHint().height()<< std::endl;
    std::cout << "WDispo: "<<  Qevent->size().width() - menus->sizeHint().width()<<
                 " HDispo: "<< Qevent->size().height() - menus->sizeHint().height()<< std::endl;
#endif
}


//################################
/***		Slots				***/
//################################
/*** Timers ***/
void FireScreen::start_timer()
{
    timer->start(delai);
    next_btn->setEnabled(false);
    play_btn->setEnabled(false);
    pause_btn->setEnabled(true);
}

void FireScreen::stop_timer()
{
    timer->stop();
    next_btn->setEnabled(true);
    play_btn->setEnabled(true);
    pause_btn->setEnabled(false);
}

/*** 	Sliders	***/
void FireScreen::set_delai(int x)
{
    delai = (long)x;
    delai_lbl->setText(QString::number(x));
    timer->setInterval(delai);
}

/*** 	Boutons	***/
void FireScreen::nextStep()
{
    if (fWidget->next()){
        nb_tour += 1;
        majCompteur();
        windWidget->changeWindDir();
    }
    else stop_timer();
}

void FireScreen::reset()
{
    stop_timer();

    //Fwelcome fwel(this, fWidget->getForet()->width(), fWidget->getForet()->height());
    fwel->addCancel();
    fwel->setModal(true);

    if ( tryInitForest() ){
        fWidget->redraw();
    }
}

// SAUVEGARDES
void FireScreen::checkInitFileSaveDialog(){
    if (fileSaveDialog == NULL) {
        fileSaveDialog = new QFileDialog(this);
        fileSaveDialog->setAcceptMode(QFileDialog::AcceptSave);
    }
}

void FireScreen::saveData()
{
    checkInitFileSaveDialog();
    fileSaveDialog->setNameFilter(tr("Sauvegarde") +" (*.data *.dat *.frt *.sav *.save)");

    string s;
    if(fileSaveDialog->exec() == QDialog::Accepted){
        QFileInfo fileInfo(fileSaveDialog->selectedFiles().at(0));
        cout <<"taille de "<< s<< " : "<< s.length() << endl;
        if (fileInfo.suffix()== ".dat" ||fileInfo.suffix()==  ".data" || fileInfo.suffix()==  ".frt" || fileInfo.suffix()==  ".sav" || fileInfo.suffix()==  ".save"){
            s= fileInfo.bundleName().toStdString();
        }
        else  s= fileInfo.completeBaseName().toStdString() + ".dat";

        // Sauvegarde de la foret dans FireWidget qui effectue la procédure de Foret
        fWidget->trySaveForest(s);
    }
}

void FireScreen::saveImage()
{
    checkInitFileSaveDialog();
    fileSaveDialog->setNameFilter(tr("Images") +" (*.png *.jpg *.jpeg *.tif *.tiff *.bmp)");

    string s;
    if(fileSaveDialog->exec() == QDialog::Accepted){
        s= fileSaveDialog->selectedFiles()[0].toStdString();
        cout <<"taille de "<< s<< " : "<< s.length() << endl;

        // Sauvegarde de la foret dans FireWidget qui effectue la procédure de Foret
        fWidget->trySaveImage(QString::fromStdString(s));
    }
}

void FireScreen::saveSeed()
{
    checkInitFileSaveDialog();
    fileSaveDialog->setNameFilter(tr("Sauvegarde") +" (*.seed)");

    string s;
    if(fileSaveDialog->exec() == QDialog::Accepted){
        s = fileSaveDialog->selectedFiles()[0].toStdString();
        cout <<"taille de "<< s<< " : "<< s.length() << endl;

        // Sauvegarde de la foret dans FireWidget qui effectue la procédure de Foret
        fWidget->trySaveSeed(s);
    }
}

void FireScreen::setLangEN()
{
    QLocale lang("en");
    tryChangeLanguage(lang);
}

void FireScreen::setLangFR()
{
    QLocale lang("fr");
    tryChangeLanguage(lang);
}

void FireScreen::setLangDE()
{
    QLocale lang("de");
    tryChangeLanguage(lang);
}


void FireScreen::popAbout()
{
    if(aboutWidget == NULL){
        aboutWidget = new QWidget();
        QVBoxLayout* VLay= new QVBoxLayout(aboutWidget);
        QLabel* signature= new QLabel("Florian DAVID et Ugo RAYER");
        QLabel* about= new QLabel("Projet de 3eme annee de licence");
        QLabel* coordonnees= new QLabel("f.david5@laposte.net et ugo.rayer@laposte.net");
        QLabel* depot= new QLabel("Depot : github.com/flodavid/incendie");
        QPushButton* valid= new QPushButton("OK", aboutWidget);
        VLay->addWidget(signature);
        VLay->addWidget(about);
        VLay->addWidget(coordonnees);
        VLay->addWidget(depot);
        VLay->addWidget(valid);

        signature->setStyleSheet("QLabel {color : darkblue;  }");
        about->setStyleSheet("QLabel {color : darkblue;  }");
        coordonnees->setStyleSheet("QLabel {color : darkblue;  }");
        depot->setStyleSheet("QLabel {color : darkblue;  }");
        connect(valid, SIGNAL(clicked()), aboutWidget, SLOT(close()) );
    }
    aboutWidget->show();
}


/*** Coupure et retardateur ***/
void FireScreen::releaseOrdered()
{
    if(actionBox->currentIndex() == 0){
#if DEBUG_RETARD
        std::cout << "Rien à faire"<< endl;
#endif
    }
    else if(actionBox->currentIndex() == 1){
#if DEBUG_RETARD
        std::cout << "demande de coupure" << std::endl;
#endif
        emit actionSender(CUT);
    }
    else if(actionBox->currentIndex() == 2){
#if DEBUG_RETARD
        std::cout << "demande de retardateur" << std::endl;
#endif
        emit actionSender( DELAY );
    }
}
