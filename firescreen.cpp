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

// using namespace std; // TODO remove it : utilisé seulement pour les debug


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

	delete delai_lbl;
	delete cpt_lbl;
	
	delete pause_btn;
	delete play_btn;
	delete next_btn;
	
	delete  timer;
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

void FireScreen::initMenus(QHBoxLayout* HLayout)
{
	QWidget* ww = new QWidget();
	HLayout->addWidget(ww);
	// Propriétés utiles?
	// 	ww->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	// 		ww->setMaximumWidth(350);
	// 		ww->setMinimumWidth(350);
	
	QVBoxLayout* vert_lay1 = new QVBoxLayout(ww);
	QWidget* ww1 = new QWidget(ww);
	QWidget* ww2 = new QWidget(ww);
	
	QGridLayout* grid_lay1 = new QGridLayout(ww1);
	QGridLayout* grid_lay2 = new QGridLayout(ww2);   
	QWidget* www = new QWidget(ww2);
	
	QHBoxLayout* h_lay1 = new QHBoxLayout(www);
	
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
	// 	majTimer();
	
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
	
	vert_lay1->addWidget(titre);
	vert_lay1->addWidget(info_vent);
	vert_lay1->addWidget(windWidget);
	vert_lay1->addWidget(trans_p2p);
	vert_lay1->addWidget(ww1);
	vert_lay1->addWidget(trans_con);
	vert_lay1->addWidget(ww2);
	vert_lay1->addWidget(www);
	vert_lay1->addWidget(reset_btn);
	
	vert_lay1->addStretch(2);
	vert_lay1->setAlignment(titre,Qt::AlignHCenter);
	
	// CONNEXION DES BOUTONS AUX FONCTIONS
	QObject::connect(next_btn,		SIGNAL(clicked()), fWidget,		SLOT(next()) );
	QObject::connect(play_btn,		SIGNAL(clicked(bool)), this,	SLOT(start_timer(bool)) );
	QObject::connect(pause_btn,		SIGNAL(clicked(bool)), this,	SLOT(stop_timer(bool)) );
	QObject::connect(timer, 			SIGNAL(timeout()), this,			SLOT(nextCompteur()) );
	QObject::connect(next_btn, 		SIGNAL(clicked()), this,			SLOT(nextCompteur()) );
	
	QObject::connect(slider,	SIGNAL(valueChanged(int)), this, SLOT(set_delai(int )));
	QObject::connect(reset_btn,	SIGNAL(clicked()), this,	SLOT(reset()) );
	connect(windWidget, SIGNAL(modif_value(int)), this, SLOT(updateWind(int)) );
	
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
	w = new QWidget(this);
	setCentralWidget(w);
	
	// Sous-conteneurs
	QHBoxLayout* lay = new QHBoxLayout(w);
	
	// Tests rubberBand
	rubber = NULL;
	
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

void FireScreen::initSizes(int largeur, int hauteur)
{
	
	// Maximums
	int freePixWidth= QApplication::desktop()->screenGeometry().width() -250-15; // les 15 pixels car il doit y avoir des marges (observé 14)
	int freePixHeight= QApplication::desktop()->screenGeometry().height()-45 ; // 45 pixel à cause des marges et menu (observé 43)
	
	int maxCellWidth= freePixWidth/largeur;
	int maxCellHeight= freePixHeight/hauteur; 
	int tCellMax= std::min(maxCellWidth, maxCellHeight);
	
	setMaximumWidth( tCellMax * largeur +250 +25 );
	setMaximumHeight( tCellMax * hauteur +45 );
	
	// 	resize(lay->sizeHint().height()+250 +10, lay->sizeHint().height() +menuBar()->sizeHint().height());
	// TODO mettre une largeur de base minimum, à partir de la hauteur du menu droite (calculer la taille d'une cellule si la hauteur de la fenetre est la hauteur du menu)
	resize( (tCellMax+1)/2 * largeur +250 +15, (tCellMax + 1)/2 * hauteur +45 );
	
	#if DEBUG_DIMENSION
	std::cout<< "taille cell max : "<< tCellMax<< std::endl;
	std::cout<< "largeur : "<< maxCellWidth *largeur<< 	"px \tsur : "<< freePixWidth	<<	" dispos"<<std::endl;
	std::cout<< "hauteur : "<< maxCellHeight *hauteur<<	"px \tsur : "<< freePixHeight	<<	" dispos"<<std::endl;
	std::cout<< std::endl;
	#endif
}

/**
 * Met à l'affichage du timer, utilise nb_tour
 * @author Florian et Ugo
 */
void FireScreen::majTimer()
{
	cpt_lbl->setText(QString::number(nb_tour));
}

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


// #############
// 	Events
// #############
void FireScreen::resizeEvent(QResizeEvent* Qevent)
{
// 	QWidget::resizeEvent(Qevent);
}

void FireScreen::mousePressEvent(QMouseEvent* event)
{
	if(event->button() == Qt::RightButton){
		origin = event->pos();
		if(!rubber)
			rubber = new QRubberBand(QRubberBand::Rectangle,fWidget);
		rubber->setGeometry(QRect(origin,QSize()));
		rubber->show();
		
	}
}

void FireScreen::mouseMoveEvent(QMouseEvent* event)
{
	if(rubber){
		rubber->setGeometry(QRect(origin, event->pos()).normalized());
		fWidget->update();
	}
}

void FireScreen::mouseReleaseEvent(QMouseEvent* event)
{
	if(rubber){
		rubber->hide();
	}
}


// ########################
/***			Slots			***/
// ########################
void FireScreen::start_timer(bool b)
{
	timer->start(delai);
	next_btn->setEnabled(false);
	play_btn->setEnabled(false);
	pause_btn->setEnabled(true);
}

void FireScreen::stop_timer(bool )
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
	stop_timer(true);
	
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
	else stop_timer(true);
}

void FireScreen::updateWind(int y){
	// Les paramètres du vent ont été modifiés
	// On doit récupérer les valeurs et mettre à jour le vent
	int angle = windWidget->get_angle();
	int vitesse = windWidget->get_vitesse();
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
	fWidget->setVent(horizontal,vertical);
	
	#if DEBUG_VENT
	std::cout << "Vitesse : "<< vitesse<< "; Angle : "<<angle<<" ; valeur horizontal : " << horizontal << " ; valeur vertical : " << vertical << std::endl;
	#endif
}
