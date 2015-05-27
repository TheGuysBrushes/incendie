#include "firescreen.h"

// Composants Qt qui ne sont pas des attributs de classe
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QSlider>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>

using namespace std;

/* - TODO voir pour choisir une meilleur taille initiale */

/**
 * On initialise les composant graphiques de la classe (foret et menus, boutons), la barre des menus
 * @author Ugo et Florian
 */
FireScreen::FireScreen(): QMainWindow()
{
	// TODO Version anglaise
	// Elements de la barre de menus
	QAction* exit = new QAction(this);
	exit->setText( "Quit" );
	menuBar()->addAction(exit);
	connect(exit, 	SIGNAL(triggered()), SLOT(close()) );
	
	QAction* save = new QAction(this);
	save->setText( "Save" );
	menuBar()->addAction(save);
	connect(save,	SIGNAL(triggered()), SLOT( save()) );

	// Composants Qt de la classe
	fWidget= new FireWidget();
	windWidget = new WindWidget();
	menus = new QWidget();
	timer = new QTimer();
	
#if FRENCH
	next_btn = new QPushButton("Suivant");
	play_btn = new QPushButton("Démarrer");
	pause_btn = new QPushButton("Pause");
	cut_btn = new QPushButton("Coupure");
	delay_btn = new QPushButton("Retardateur");
#else
	next_btn = new QPushButton("Next");
	play_btn = new QPushButton("Play");
	pause_btn = new QPushButton("Pause");
	cut_btn = new QPushButton("Cutting");
	delay_btn = new QPushButton("Retardatorr");
#endif

	pause_btn->setDisabled(true);
	cut_btn->setEnabled(true);
	delay_btn->setEnabled(false);
	
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

	delete timer;
	delete menus;
	delete fWidget;
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
	
	// 	resize(lay->sizeHint().height()+250 +10, lay->sizeHint().height() +menuBar()->sizeHint().height());
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
	
	h_lay2->addWidget(cut_btn);
	h_lay2->addWidget(delay_btn);
	
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
	// IMPROVEIT Temporaire, à revoir lors de la création de l'explorateur de fichier

	connect(save_btn,	SIGNAL(clicked()),	this, SLOT( save()) );	
		/* Clic droit */
	connect(cut_btn,	SIGNAL(clicked()),		this, SLOT(invertActionRightMouse()));	
	connect(delay_btn,	SIGNAL(clicked()),	this, SLOT(invertActionRightMouse()));
		/* Déroulement*/
	connect(next_btn,	SIGNAL(clicked()), 		this,	SLOT( nextStep()) );
	connect(play_btn,	SIGNAL(clicked()), 		this,	SLOT( start_timer()) );
	connect(pause_btn,	SIGNAL(clicked()),	this,	SLOT( stop_timer()) );
	
	// Autres signaux déroulement
	connect(timer,	SIGNAL(timeout()), this,	SLOT( nextStep()) );
	
	// Coupure et retardateur
	connect(fWidget,	SIGNAL(releaseSignal()),	this, SLOT(releaseOrdered()));
	connect(this,	SIGNAL(actionSender(int)),		fWidget, SLOT(actionReceived(int)));
	
	
/*** 	DEFINITTION DU STYLE DES ELEMENTS	***/
	// Touches d'améliorations visuelles et d'initialisation de propriétés
	titre->setStyleSheet("color : darkblue; font : bold italic 20px;");
	trans_con->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	trans_p2p->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	info_vent->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	tour_lbl->setStyleSheet("QLabel {  color : darkblue; }");
	cpt_lbl->setStyleSheet("QLabel { color : darkblue; }");
	delai_lbl->setStyleSheet("QLabel { color : darkblue; }");

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
 * Restaure une forêt à partir d'un fichier ouvert
 * @author Florian
 * @param file fichier binaire ouvert contenant la sauvegarde de la forêt (taille-essences-arbres)
 */
// void FireScreen::createForest(ifstream* file)
// {
// 	int largeur, hauteur;
// 	file->read( (char *)&(largeur), sizeof(int));
// 	file->read( (char *)&(hauteur), sizeof(int));
// 	
// 	createForest(largeur, hauteur, file);
// }


#include <QtGui/QImage>

/**
 * Crée une nouvelle forêt à partir d'une fenêtre de paramétrage
 * @author Florian
 * @param fwel fenêtre qui a été appelée et contient les paramètres de la nouvelle forêt
 */
void FireScreen::initForest(Fwelcome * fwel)
{
	nb_tour = 0;
	int largeur= fwel->get_larg();
	int hauteur= fwel->get_haut();
	ifstream* file= fwel->getFile();
	QImage* picture= fwel->getImage();	
	
	if ( file->is_open()){ // Creation par fichier de sauvegarde si il est ouvert
		fWidget->initialise(largeur,hauteur, file);
	}
	else {
		#if DEBUG_CREA_FORET
		cout<< "Pas de fichier, essai création par image"<< endl;
		#endif
		
		if (!picture->isNull()){ // Creation par image si elle est chargée
			fWidget->initialise(picture);
		}
		else { // Sinon, cas par défaut : Creation d'une forêt selon les paramétres de la fenetre d'initialisation
			#if DEBUG_CREA_FORET
			cout << "Pas d'image, creation foret à partir des parametres de fwelcome"<< endl;
			#endif
			
			fWidget->initialise(largeur,hauteur,
									fwel->get_proba(),
									fwel->get_coef()	);
		}
		
		majCompteur();
	}
	
	initSizes(largeur, hauteur);
}


/**
 * Crée une nouvelle forêt à partir d'une fenêtre de paramétrage
 * @author Florian et Ugo
 */
bool FireScreen::initialisation()
{
	initComponents();
	
	Fwelcome* fwel = new Fwelcome(this);
	fwel->show();
	
	if( fwel->exec() == QDialog::Accepted ){
		initForest(fwel);
		
		return true;
	}
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
// TODO mettre dans le .h ?
void FireScreen::updateWind(int angle, int vitesse)
{
	fWidget->setWind(angle, vitesse);
}

/*** 	Boutons	***/

/**
 * Appelle une nouvelle fenêtre de paramétrage de forêt.
 * Si l'utilisateur valide, alors une nouvelle forêt,
 * sinon, l'ancienne forêt est conservée
 * @author Florian et Ugo
 */
void FireScreen::reset()
{
	stop_timer();
	
	Fwelcome* fwel = new Fwelcome(this);
	fwel->addCancel();
	fwel->setModal(true);
	fwel->show();
	
	if( fwel->exec() == QDialog::Accepted ){
		fWidget->delForest();
		
		initForest(fwel);
		fWidget->razRubber();
// 		fWidget->redraw();
	}
}

/**
 * Appelle la sauvegarde de la foret de fWidget 
 * @author Florian
 */
void FireScreen::save() const
{
	fWidget->saveForest();
}

/**
 * Cette fonction permet d'inverser les actions effectuées par le clic droit.
 * @author Ugo
 */
void FireScreen::invertActionRightMouse()
{
	if(cut_btn->isEnabled()){
		cut_btn->setDisabled(true);
		delay_btn->setEnabled(true);
	}else{
		delay_btn->setDisabled(true);
		cut_btn->setEnabled(true);
	}
	
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
	// L'action choisie est représenté par le fait que le boutton est désactivé
	if(!cut_btn->isEnabled()){
		#if DEBUG_RETARD
		std::cout << "demande de coupure" << std::endl;
		#endif
		emit actionSender(CUT);
	} else if(!delay_btn->isEnabled()){
		#if DEBUG_RETARD
		std::cout << "demande de retardateur" << std::endl;
		#endif
		emit actionSender(DELAY);
	}
	
}