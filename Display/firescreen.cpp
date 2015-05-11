#include "firescreen.h"

// #include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtCore/QString>
#include <QtGui/QSlider>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QApplication>

#include <qdesktopwidget.h>

#include <math.h>
#define PI 3.14159265

// using namespace std; // REMOVEIT ? : utilisé seulement pour les debug


/*		
 * - TODO voir pour choisir une meilleur taille initiale
 */
FireScreen::FireScreen(): QMainWindow()
{
	QAction* exit = new QAction(this);
	exit->setText( "Quitter" );
	
	QAction* save = new QAction(this);
	save->setText( "Save" );
	
	fWidget= new FireWidget();
	windWidget = new WindWidget();
	
	menuBar()/*->addMenu( "Menu" )*/->addAction(exit);
	menuBar()->addAction(save);
	connect(exit, SIGNAL(triggered()), SLOT(close()) );
	
	timer = new QTimer();
	
	next_btn = new QPushButton("Next");
	play_btn = new QPushButton("Play");
	pause_btn = new QPushButton("Pause");
	cut_btn = new QPushButton("Coupure");
	delay_btn = new QPushButton("Retardateur");
	pause_btn->setDisabled(true);
	
	cpt_lbl = new QLabel();
	delai_lbl = new QLabel();
}

// FireScreen::FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure/*, QWidget* parent, Qt::WindowFlags flags*/)
// :	/*QMainWindow(parent, flags),*/	delai(nTemps)
// {
// 	initialiseFixes();
// 	initialiseParametres(hauteur, largeur, proba, nTemps, coef_brulure);
// }

FireScreen::~FireScreen()
{
	delete(fWidget);	
	
	delete menus;

	delete delai_lbl;
	delete cpt_lbl;
	
	delete pause_btn;
	delete play_btn;
	delete next_btn;
	
	delete  timer;
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
	
	setMaximumWidth( tCellMax * largeur +largeurMenu +25 );
	setMaximumHeight( tCellMax * hauteur +45 );
	
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


void FireScreen::initMenus(QHBoxLayout* HLayout)
{
	/*QWidget* */menus = new QWidget();
	HLayout->addWidget(menus);
	// Propriétés utiles?
		menus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
// 			menus->setMaximumWidth(300);
// 			menus->setMinimumWidth(300);
	
	QVBoxLayout* vert_lay1 = new QVBoxLayout(menus);
	QWidget* ww1 = new QWidget(menus);
	QWidget* ww2 = new QWidget(menus);
	QWidget* ww3 = new QWidget(menus);
	
	QGridLayout* grid_lay1 = new QGridLayout(ww1);
	QGridLayout* grid_lay2 = new QGridLayout(ww2); 
	
	QWidget* www = new QWidget(ww2);
	
	QHBoxLayout* h_lay1 = new QHBoxLayout(www);
	QHBoxLayout* h_lay2 = new QHBoxLayout(ww3);
	
	//ELEMENTS
	
	// Element du panneau de direction de l'automate
	QLabel* titre = new QLabel("Automate cellulaire");
	QLabel* info_vent= new QLabel("Parametres du vent :");
	
	// Boutons
	QLabel* trans_p2p = new QLabel("Transmission pas-a-pas : ");
	
	QPushButton* reset_btn = new QPushButton("Reset ! Be careful");
	
	int interTempsInit= 200;
	
	// Compteur de tours et Slider
	QLabel* trans_con = new QLabel("Transmission continue : ");	
	QLabel* tour_lbl = new QLabel("Nombre de tours :");
	// 	majTimer(); REMOVEIT ?
	
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
	vert_lay1->addWidget(reset_btn);
	
	vert_lay1->addStretch(2);
	vert_lay1->setAlignment(titre,Qt::AlignHCenter);
	
	// CONNEXION DES BOUTONS AUX FONCTIONS
	connect(next_btn,	SIGNAL(clicked()), 		fWidget,	SLOT(next()) );
	connect(play_btn,	SIGNAL(clicked()), 		this,	SLOT( start_timer()) );
	connect(pause_btn,	SIGNAL(clicked()),	this,	SLOT( stop_timer()) );
	connect(timer,		SIGNAL(timeout()), 		this,	SLOT( nextCompteur()) );
	connect(next_btn,	SIGNAL(clicked()), 		this,	SLOT( nextCompteur()) );
	
	connect(slider,	SIGNAL(valueChanged(int)),		this, SLOT( set_delai(int)) );
	connect(reset_btn,	SIGNAL(clicked()), 			this,	SLOT( reset()) );
	connect(windWidget,	SIGNAL(modif_value(int, int)),	this, SLOT( updateWind(int, int)) );
	
	
	QObject::connect(cut_btn,	SIGNAL(clicked(bool)),		this, SLOT(invertBtn(bool)));	
	QObject::connect(delay_btn,	SIGNAL(clicked(bool)),	this, SLOT(invertBtn(bool)));
	
	// Connexion pour coupure et retardateur
	QObject::connect(fWidget, SIGNAL(releaseSignal()), this, SLOT(releaseOrdered()));
	QObject::connect(this, SIGNAL(actionSender(int)), fWidget, SLOT(actionReceived(int)));
	
	
	cut_btn->setEnabled(true);
	delay_btn->setEnabled(false);
	
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

void FireScreen::initForest(const Fwelcome* fwel)
{
	nb_tour = 0;
	
	int largeur = fwel->get_larg();
	int hauteur = fwel->get_haut();
	fWidget->initialise(largeur,hauteur,
							  fwel->get_proba(),
							  fwel->get_coef());
	
	majTimer();
	initSizes(largeur, hauteur);
}


/**
 * 
 * 
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
 * Cette fonction permet d'inverser les actions effectuées par le clic
 * droit.
 * @author Ugo
 */
void FireScreen::invertBtn(bool )
{
		if(cut_btn->isEnabled()){
			cut_btn->setDisabled(true);
			delay_btn->setEnabled(true);
		}else{
			delay_btn->setDisabled(true);
			cut_btn->setEnabled(true);
		}
	
}

/**
 * Met à l'affichage du timer, utilise nb_tour
 * @author Florian et Ugo
 */
void FireScreen::majTimer()
{
	cpt_lbl->setText(QString::number(nb_tour));
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
	std::cout << "WDispo: "<<  Qevent->size().width() - menus->sizeHint().width()<< " HDispo: "<< Qevent->size().height() - menus->sizeHint().height()<< std::endl;
	
	#endif
}

// ########################
/***			Slots			***/
// ########################
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

void FireScreen::set_delai(int x)
{
	delai = (long)x;
	delai_lbl->setText(QString::number(x));
	timer->setInterval(delai);
}



void FireScreen::reset()
{
	stop_timer();
	
	Fwelcome* fwel = new Fwelcome(this);
	fwel->get_cancel()->setVisible(true);
	fwel->setModal(true);
	fwel->show();
	
	if( fwel->exec() == QDialog::Accepted ){
		fWidget->delForet();
		
		initForest(fwel);
		
		fWidget->redraw();
	}
}

void FireScreen::nextCompteur()
{
	if (fWidget->next()){
		nb_tour += 1;
		majTimer();
	}
	else stop_timer();
}

void FireScreen::updateWind(int angle, int vitesse){
	// Les paramètres du vent ont été modifiés
	// On doit récupérer les valeurs et mettre à jour le vent
	float vertical;
	
	float horizontal;
	if( (angle >= 90 && angle <180) || (angle >=270 && angle < 360)){
		vertical = sin(PI*(float)(angle+180.0)/180.0);
		horizontal = cos(PI*(float)(angle+180)/180.0);
	}else{
		vertical = sin(PI*(float)angle/180.0);
		horizontal = cos(PI*(float)angle/180.0);
	}
	if(vitesse >50 && vitesse <=100){
		horizontal *= 3.0;
		vertical *= 3.0;
	}else if(vitesse > 100){
		horizontal *= 4.0;
		vertical *= 4.0;
	}else{
		horizontal*=2.0;
		vertical*=2.0;
	}
	fWidget->setWind(horizontal,vertical);
	
	#if DEBUG_VENT
	std::cout << "Vitesse : "<< vitesse<< "; Angle : "<<angle<<" ; valeur horizontal : " << horizontal << " ; valeur vertical : " << vertical << std::endl;
	#endif
}

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
		emit actionSender(0);
	}else if(!delay_btn->isEnabled()){
		emit actionSender(1);
	}
	
}
