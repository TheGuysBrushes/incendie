#include "firescreen.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtCore/QString>
#include <QtGui/QSlider>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
/*		BUG 
 *  - Lorsque le clic est enfoncé et que l'on sort du cadre du widget, Arrêt brutal 
 * 	TODO CORRIGE ?
 * 
 */
FireScreen::FireScreen(): QMainWindow()
{
	QAction* exit = new QAction(this);
	exit->setText( "Quitter" );
	
	QAction* save = new QAction(this);
	save->setText( "Save" );
	
	
	menuBar()/*->addMenu( "Menu" )*/->addAction(exit);
	menuBar()->addAction(save);
	connect(exit, SIGNAL(triggered()), SLOT(close()) );
	
	nb_tour = 0;
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
	delete(fwidget);	

	delete delai_lbl;
	delete cpt_lbl;
	
	delete pause_btn;
	delete play_btn;
	delete next_btn;
	
	delete  timer;
}

void FireScreen::initialiseParametres(int hauteur, int largeur, float proba, float coef_brulure/*, QWidget* parent, Qt::WindowFlags flags*/)
{
	// AFFICHEUR DE FORET
	fwidget= new FireWidget(hauteur, largeur, proba, coef_brulure);
	
/*** 	BOUTONS ET INTERFACE		***/
	
	// CONTENEURS
	// Conteneur général
	QWidget* w = new QWidget(this);
	
	// Sous-conteneurs
	QHBoxLayout* lay = new QHBoxLayout(w);
	QWidget* ww = new QWidget(w);
	// Propriétés utiles?
	ww->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	// 	ww->setMaximumWidth(250);
	// 	ww->setMinimumWidth(250);
	
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
	
	// Boutons
	QLabel* trans_p2p = new QLabel("Transmission pas-a-pas : ");
	
	QPushButton* reset_btn = new QPushButton("Reset ! Be careful");
	
	
	int interTempsInit= 200;

	// Compteur de tours et Slider
	QLabel* trans_con = new QLabel("Transmission continue : ");
	
	QLabel* tour_lbl = new QLabel("Nombre de tours :");
	majTimer();
	
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
	vert_lay1->addWidget(trans_p2p);
	vert_lay1->addWidget(ww1);
	vert_lay1->addWidget(trans_con);
	vert_lay1->addWidget(ww2);
	vert_lay1->addWidget(www);
	vert_lay1->addWidget(reset_btn);
	
	vert_lay1->addStretch(2);
	vert_lay1->setAlignment(titre,Qt::AlignHCenter);
	
	// PLACEMENT DES ELEMENTS
	lay->addWidget(fwidget);
	lay->setStretchFactor(fwidget, 1);
	lay->minimumHeightForWidth(1);
	lay->addWidget(ww);
	setCentralWidget(w);
	
	// CONNEXION DES BOUTONS AUX FONCTIONS
	QObject::connect(next_btn,		SIGNAL(clicked()), fwidget,	SLOT(next()) );
	// 	QObject::connect(timer,			SIGNAL(timeout()), fwidget, 	SLOT(next()) ); // IMPROVEIT vraiment util d'utiliser signal->slot ?
	QObject::connect(play_btn,		SIGNAL(clicked(bool)), this,	SLOT(start_timer(bool)) );
	QObject::connect(pause_btn,	SIGNAL(clicked(bool)), this, SLOT(stop_timer(bool)) );
	QObject::connect(timer, 		SIGNAL(timeout()), this,		SLOT(compteur()) );
	QObject::connect(next_btn, 	SIGNAL(clicked()), this,	SLOT(compteur()) );
	
	QObject::connect(slider,	SIGNAL(valueChanged(int)), this, SLOT(set_delai(int )));
	
	/// @author Florian
	QObject::connect(reset_btn,	SIGNAL(clicked()), this,	SLOT(reset()) );
	
	// Tests pour RAZ de la matrice
	// 	QObject::connect(this, SIGNAL(ask_restart()), fwidget, SLOT(restart()));
	
	
/*** 	DEFINITTION DU STYLE DES ELEMENTS	***/
	// Touches d'améliorations visuelles et d'initialisation de propriétés
	titre->setStyleSheet("color : darkblue; font : bold italic 20px;");
	trans_con->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	trans_p2p->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	tour_lbl->setStyleSheet("QLabel {  color : darkblue; }");
	cpt_lbl->setStyleSheet("QLabel { color : darkblue; }");
	delai_lbl->setStyleSheet("QLabel { color : darkblue; }");
	
	
// 	Definition de la taille selon les éléments

	// taille minimale de la fenetre : (Hpanneau + Lpanneau)/(Hpanneau) + marges,
	//		ce qui donne un carré à gauche de cote au moins la hauteur du panneau, pour la foret, ET le panneau à droite
	setMinimumSize(lay->sizeHint().height()+250 +10, lay->sizeHint().height() +menuBar()->sizeHint().height());
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
	Fwelcome* fwel = new Fwelcome(this);
	fwel->cancel_btn->setVisible(true);
	fwel->show();
	if(fwel->exec() == QDialog::Accepted ){
		stop_timer(true);
		nb_tour= 0;
		majTimer();
		int hauteur = fwel->get_haut();
		int largeur = fwel->get_larg();
		float proba = fwel->get_proba();
		float coef_brulure = fwel->get_coef();
	
		fwidget->reset(hauteur,largeur,proba,coef_brulure);
	}

}


void FireScreen::compteur()
{
	// IMPROVEIT tour suivant fait dans le compteur pour l'instant
	if (fwidget->next()){
		nb_tour += 1;
		majTimer();
	}
	else stop_timer(true);
}
