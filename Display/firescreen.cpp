#include "firescreen.h"

// Composants Qt qui ne sont pas des attributs de classe
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QSlider>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QImage>

using namespace std;

/**
 * On initialise les composant graphiques de la classe (foret et menus, boutons), la barre des menus
 * @author Ugo et Florian
 */
FireScreen::FireScreen(): QMainWindow()
{
	// Elements de la barre de menus
	QAction* exit = new QAction(this);
	#if FRENCH
	exit->setText( "Quitter" );
	#else
	exit->setText( "Quit" );
	#endif
	menuBar()->addAction(exit);
	connect(exit, 	SIGNAL(triggered()), SLOT(close()) );
	
	QAction* save = new QAction(this);
	#if FRENCH
	save->setText( "Enregistrer" );
	#else
	save->setText( "Save" );
	#endif

	// Composants Qt de la classe
	fWidget= new FireWidget();
	windWidget = new WindWidget();
	menus = new QWidget();
	timer = new QTimer();
	// COMBOBOX des différentes actions
	actionBox = new QComboBox(this);
	actionBox->addItem("");
	

#if FRENCH
	next_btn = new QPushButton("Suivant");
	play_btn = new QPushButton("Démarrer");
	pause_btn = new QPushButton("Pause");
	actionBox->addItem("Coupure");
	actionBox->addItem("Retardateur");	
#else
	next_btn = new QPushButton("Next");
	play_btn = new QPushButton("Play");
	pause_btn = new QPushButton("Pause");
	actionBox->addItem("Cutting");
	actionBox->addItem("Retardator");	

#endif

	pause_btn->setDisabled(true);
	
	fileSaveDialog = NULL;
	
	cpt_lbl = new QLabel();
	delai_lbl = new QLabel();
}

FireScreen::~FireScreen()
{
	delete delai_lbl;
	delete cpt_lbl;
	
	delete pause_btn;
	delete play_btn;
	delete next_btn;

	delete actionBox;
	delete timer;
	delete menus;
	delete fWidget;
	
	delete fileSaveDialog;
}


/**
 * Definit les tailles maximales de la fenetre
 * @author Florian et Ugo
 * @param largeur nombre de cases de la matrice en largeur
 * @param hauteur nombre de cases en hauteur
 */
void FireScreen::initSizes(int largeur, int hauteur)
{
	int largeurMenu= menus->sizeHint().width();
	
	// Maximums
	int freePixWidth= QApplication::desktop()->screenGeometry().width() -25; // les 25 pixels car il doit y avoir des marges (observé 14 puis 24 ??)
	int freePixHeight= QApplication::desktop()->screenGeometry().height()-45 ; // 45 pixel à cause des marges et menu (observé 43)
	
	int maxCellWidth= (freePixWidth - largeurMenu) / largeur;
	int maxCellHeight= freePixHeight/hauteur; 
	int tCellMax= std::min(maxCellWidth, maxCellHeight);
	
// 	setMaximumWidth( tCellMax * largeur +largeurMenu +25 );
// 	setMaximumHeight( tCellMax * hauteur +45 );
// 	setMaximumHeight( freePixHeight );
	
// 		resize(lay->sizeHint().height()+250 +10, lay->sizeHint().height() +menuBar()->sizeHint().height());
	// TODO mettre une largeur de base minimum, à partir de la hauteur du menu droite (calculer la taille d'une cellule si la hauteur de la fenetre est la hauteur du menu)
	resize( (tCellMax+1)/2 * largeur + largeurMenu +25, (tCellMax + 1)/2 * hauteur +45 );
	
	/// ATTENTION les valeurs max sont redéfinies dans ce debug, il faut transposer les valeurs correctes au dessus (pour "performances")
	#if DEBUG_DIMENSION
	std::cout<< "taille cell max : "<< tCellMax<< std::endl;
	
	int maxLarg= tCellMax * largeur + largeurMenu +25;
	setMaximumWidth( maxLarg);
	std::cout<< "larg max window "<< maxLarg<< "px sur "<< freePixWidth	<< " dont "<< largeurMenu<< " menus, dispos"<<std::endl;
	
	int maxHaut= tCellMax * hauteur +45;
	setMaximumHeight(maxHaut);
	std::cout<< "haut max window "<< maxHaut<<	"px sur "<< freePixHeight	<<	" dispos"<<std::endl;
	std::cout<< std::endl;
	#endif
}

/**
 * Crée les menus sur la droite de la fenêtre et les place dans un layout
 * @author Ugo (organisation Florian)
 * @param HLayout layout horizontal dans lequel on place les menus
 */
void FireScreen::initMenus(QHBoxLayout* HLayout)
{
	HLayout->addWidget(menus);
	// Propriétés utiles?
		menus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
// 			menus->setMaximumWidth(300);
// 			menus->setMinimumWidth(300);
	
	QVBoxLayout* vert_lay1 = new QVBoxLayout(menus);
	QWidget* ww1 = new QWidget(menus);
	QWidget* ww2 = new QWidget(menus);
	QWidget* ww3 = new QWidget(menus);
	QWidget* ww4 = new QWidget(menus);
	
	QGridLayout* grid_lay1 = new QGridLayout(ww1);
	QGridLayout* grid_lay2 = new QGridLayout(ww2);
	QGridLayout* grid_lay3 = new QGridLayout(ww4);
	
	QWidget* www = new QWidget(ww2);
	
	QHBoxLayout* h_lay1 = new QHBoxLayout(www);
	QHBoxLayout* h_lay2 = new QHBoxLayout(ww3);
	
	//ELEMENTS
	
	// Element du panneau de direction de l'automate
#if FRENCH
	QLabel* titre = new QLabel("Automate cellulaire");
	QLabel* info_vent= new QLabel("Parametres du vent :");
	
	// Boutons
	QLabel* trans_p2p = new QLabel("Transmission pas-a-pas : ");
	
	QPushButton* reset_btn = new QPushButton("RAZ ! Attention");
	QPushButton* save_btn = new QPushButton("Sauvegarder");
	QPushButton* load_btn = new QPushButton("Charger");
	// Compteur de tours et Slider
	QLabel* trans_con = new QLabel("Transmission continue : ");	
	QLabel* tour_lbl = new QLabel("Nombre de tours :");
	
#else 
	QLabel* titre = new QLabel("Cellular automaton");
	QLabel* info_vent= new QLabel("Wind's settings :");
	
	// Boutons
	QLabel* trans_p2p = new QLabel("Step-to-step transmission : ");
	
	QPushButton* reset_btn = new QPushButton("Reset ! Be careful");
	QPushButton* save_btn = new QPushButton("Save");
	
	// Compteur de tours et Slider
	QLabel* trans_con = new QLabel("Continuous transmission : ");	
	QLabel* tour_lbl = new QLabel("Number of turns :");
#endif
	
	int interTempsInit= 200;
	
	// 	majCompteur(); REMOVEIT ?
	
	// Ajouter la scrollbar horizontale
	set_delai(interTempsInit);
	
	QSlider* slider = new QSlider(Qt::Horizontal);
	slider->setMinimum(10);
	slider->setMaximum(500);
	slider->setValue(interTempsInit);	// position initiale du slider
	
	
// Ajout des éléments dans les conteneurs
	grid_lay1->addWidget(next_btn,0,0);
	
	grid_lay2->addWidget(play_btn,0,0);
	grid_lay2->addWidget(pause_btn,0,1);
	grid_lay2->addWidget(slider,1,0);
	grid_lay2->addWidget(delai_lbl,1,1);
	

	grid_lay3->addWidget(save_btn,0,0);
	grid_lay3->addWidget(reset_btn,1,0);
	
	h_lay1->addWidget(tour_lbl);
	h_lay1->addWidget(cpt_lbl);
	
	QLabel* actionLabel = new QLabel("Action a effectuer :");

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
	vert_lay1->addWidget(ww4);

	vert_lay1->setAlignment(titre,Qt::AlignHCenter);
	
/***	SIGNAUX	***/

	// Sliders
	connect(slider,	SIGNAL(valueChanged(int)),		this, SLOT( set_delai(int)) );
	connect(windWidget,	SIGNAL(modif_value(int, int)),	this, SLOT( updateWind(int, int)) );
	
	// Boutons
		/* Gestion foret */
	connect(reset_btn,	SIGNAL(clicked()), 	this,	SLOT( reset()) );
	connect(save_btn,	SIGNAL(clicked()),	this, SLOT( save()) );	
	
		/* Déroulement*/
	connect(next_btn,	SIGNAL(clicked()), 		this,	SLOT( nextStep()) );
	connect(play_btn,	SIGNAL(clicked()), 		this,	SLOT( start_timer()) );
	connect(pause_btn,	SIGNAL(clicked()),	this,	SLOT( stop_timer()) );
	
	// Autres signaux déroulement
	connect(timer,	SIGNAL(timeout()), this,	SLOT( nextStep()) );
	
	// Coupure et retardateur
	connect(fWidget,	SIGNAL(releaseSignal()),	this, SLOT(releaseOrdered()));
	connect(this,	SIGNAL(actionSender(int)),		fWidget, SLOT(actionReceived(int)));
	connect(fWidget,	SIGNAL(endAction()),			this, SLOT(start_timer()));
	
/*** 	DEFINITTION DU STYLE DES ELEMENTS	***/
	// Touches d'améliorations visuelles et d'initialisation de propriétés
	titre->setStyleSheet("color : darkblue; font : bold italic 20px;");
	trans_con->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	trans_p2p->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	info_vent->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	tour_lbl->setStyleSheet("QLabel {  color : darkblue; }");
	cpt_lbl->setStyleSheet("QLabel { color : darkblue; }");
	delai_lbl->setStyleSheet("QLabel { color : darkblue; }");
	actionLabel->setStyleSheet("QLabel { color : darkblue; }");

}

/**
 * Crée les menus et place tous les composants dans fenêtre
 * @author Ugo et Florian
 */
void FireScreen::initComponents(/*, QWidget* parent, Qt::WindowFlags flags*/)
{
/*** 	BOUTONS ET INTERFACE		***/

// CONTENEURS
	// Conteneur général
	QWidget* w = new QWidget(this);
	setCentralWidget(w);
	
	// Sous-conteneurs
	QHBoxLayout* lay = new QHBoxLayout(w);
	
// PLACEMENT DES ELEMENTS
	// Partie gauche
	lay->addWidget(fWidget);
	lay->setStretchFactor(fWidget, 1);
	lay->minimumHeightForWidth(1);
	// Partie droite, menus
	initMenus(lay);
	
// 	Definition de la taille selon les éléments
	setMinimumHeight(lay->sizeHint().height() + menuBar()->sizeHint().height());
// 	setMinimumWidth(vert_lay1->sizeHint().width());
}


/**
 * Essai de créer un nouvelle forêt grâce à une fenêtre de paramétrage (Fwelcome)
 * @author Florian
 * @param fwel fenêtre de paramétrage
 * @return faux si l'utilisateur annule ou qu'il y a une erreur
 */
bool FireScreen::initForest(Fwelcome* fwel)
{
	fwel->show();
	
	int resExec= fwel->exec();
	if (resExec>0){
		
		fWidget->delPicture();
		nb_tour = 0;
		int largeur= fwel->getLarg();
		int hauteur= fwel->getHaut();
		
		if(  resExec== Load ){
			QImage* picture= fwel->getImage();
			fWidget->initialise(largeur, hauteur, picture,fwel->getCoef());
			windWidget->initValues(30, 80);
		}
		else if(  resExec== Restore ){
			ifstream* file= fwel->getFile();
			fWidget->initialise(largeur,hauteur, file);
			windWidget->initValues(30, 80);
		}
		else if( resExec==RestoreSeed ){
			fWidget->initialise(largeur,hauteur,
									  fwel->getProba(), fwel->getCoef(), fwel->getSeed() );
			windWidget->initValues(30, 80);
		}		
		else if( resExec==QDialog::Accepted ){
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
	else return false;
}

/**
 * Initialise l'application et crée une nouvelle forêt, en utilisant une fenêtre dédiée (Fwelcome)
 * @author Florian et Ugo
 */
bool FireScreen::initialisation()
{
	initComponents();
	
	Fwelcome* fwel = new Fwelcome(this);
	
	if ( initForest(fwel) )		
		return true;
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


// #############
// 	Events
// #############
/**
 * Redéfinition de resizeEvent, utilisée seulement pour le debuggage,
 * pour connaitre la taille de la fenetre et des menus
 * @author Florian
 */
void FireScreen::resizeEvent(QResizeEvent* Qevent)
{
	QWidget::resizeEvent(Qevent);

	#if DEBUG_DIMENSION
	std::cout << "WWindow: "<< Qevent->size().width()<< " HWindow: "<< Qevent->size().height()<< std::endl;
	std::cout << "WMenu: "<< menus->sizeHint().width()<< " HMenu: "<<menus->sizeHint().height()<< std::endl;
	std::cout << "WDispo: "<<  Qevent->size().width() - menus->sizeHint().width()<< " HDispo: "<< Qevent->size().height() - menus->sizeHint().height()<< std::endl;
	#endif
}


	//################################
	/***				Slots				***/
	//################################

/*** 	Sliders	***/

/**
 * Définit le nouveau pas utilisée lors de l'avancement continue
 * @author Ugo
 */
void FireScreen::set_delai(int x)
{
	delai = (long)x;
	delai_lbl->setText(QString::number(x));
	timer->setInterval(delai);
}

/**
 * Les paramètres du vent ont été modifiés.
 * On doit récupérer les valeurs et mettre à jour le vent
 */
void FireScreen::updateWind(int angle, int vitesse)
{
	fWidget->setWind(angle, vitesse);
}

/*** 	Boutons	***/

/**
 * Une nouvelle fenêtre de création de forêt est ouverte.
 * Si l'utilisateur valide, alors une nouvelle forêt est crée,
 * sinon, l'ancienne forêt est conservée
 * @author Florian et Ugo
 */
void FireScreen::reset()
{
	stop_timer();
	
	Fwelcome* fwel = new Fwelcome(this, fWidget->getForet()->width(), fWidget->getForet()->height());
	fwel->addCancel();
	fwel->setModal(true);
	
	if ( initForest(fwel) )
		fWidget->redraw();
}    

/**
 * Appelle la sauvegarde de la foret de fWidget 
 * @author Florian
 */
void FireScreen::save()
{
	fileSaveDialog = new QFileDialog(this);
	fileSaveDialog->setAcceptMode(QFileDialog::AcceptSave);
	string s;
	if(fileSaveDialog->exec()){
		s = fileSaveDialog->selectedFiles().at(0).toStdString();
		cout << s << s.length() << endl;
	}

	fWidget->saveForest(s);
}

	/* Déroulement */
/**
 * Avance la progression de l'incendie d'un tour (t+1)
 * @author Florian
 */
void FireScreen::nextStep()
{
	if (fWidget->next()){
		nb_tour += 1;
		majCompteur();
		windWidget->changeWindDir();
	}
	else stop_timer();
}

/**
 * Démarre le déroulement continue de l'incendie
 * @author Ugo
 */
void FireScreen::start_timer()
{
	timer->start(delai);
	next_btn->setEnabled(false);
	play_btn->setEnabled(false);
	pause_btn->setEnabled(true);
}

/**
 * Démarre le déroulement continue de l'incendie
 * @author Ugo
 */
void FireScreen::stop_timer()
{
	timer->stop();
	next_btn->setEnabled(true);
	play_btn->setEnabled(true);
	pause_btn->setEnabled(false);
}

/*** Coupure et retardateur ***/

/**
 * Slot mis en place afin de transmettre l'action sélectionnée à appliquer
 * après une selection sur la matrice. Est connecté au signal émis lors du 
 * releaseMouseEvent de fwidget lorsque le clic droit était enfoncé.
 * Voir commentaire Slack pour mise en #define. Pour le moment, 
 * le signal 0 correspond à une coupure, le 1 à un retardateur.
 * @author Ugo
 */
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
		stop_timer();
		pause_btn->setEnabled(true);
		emit actionSender(CUT);
	}
	else if(actionBox->currentIndex() == 2){
		#if DEBUG_RETARD
		std::cout << "demande de retardateur" << std::endl;
		#endif
		stop_timer();
		pause_btn->setEnabled(true);
		emit actionSender( DELAY );
	}

}