#include "firescreen.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

FireScreen::FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure/*, QWidget* parent, Qt::WindowFlags flags*/)
:	/*QMainWindow(parent, flags),*/		play(false), delai(500)
{
// AFFICHEUR DE FORET
	fwidget= new FireWidget(hauteur, largeur, proba, nTemps, coef_brulure);
	timer = new QTimer();
	
// BOUTONS
	// Conteneur général
   QWidget* w = new QWidget(this);
   
   // Sous-conteneurs
   QHBoxLayout* lay = new QHBoxLayout(w);
   QWidget* ww = new QWidget(w);
   QVBoxLayout* vert_lay1 = new QVBoxLayout(ww);
   QWidget* ww1 = new QWidget(ww);
   QWidget* ww2 = new QWidget(ww);
   QGridLayout* grid_lay1 = new QGridLayout(ww1);
   QGridLayout* grid_lay2 = new QGridLayout(ww2);   
   
   // Element du panneau de direction de l'automate
   QLabel* titre = new QLabel("Automate cellulaire");
   QLabel* trans_p2p = new QLabel("Transmission pas-a-pas : ");
   QLabel* trans_con = new QLabel("Transmission continue : ");
   QPushButton* next_btn = new QPushButton("Next");
   QPushButton* play_btn = new QPushButton("Play");
   QPushButton* pause_btn = new QPushButton("Pause");
   QPushButton* reset_btn = new QPushButton("Reset ! Be careful");
   // Ajouter la scrollbar horizontale
   
	// Touches d'améliorations visuelles
	titre->setStyleSheet("color : darkblue; font : bold italic 20px;");
	trans_con->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	trans_p2p->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	
	// Ajout des éléments dans les conteneurs
	grid_lay1->addWidget(trans_p2p,0,0);
	grid_lay1->addWidget(next_btn,1,0);
	
	grid_lay2->addWidget(trans_con,0,0);
	grid_lay2->addWidget(play_btn,1,0);
	grid_lay2->addWidget(pause_btn,1,1);
   
	vert_lay1->addWidget(titre);
	vert_lay1->addWidget(ww1);
	vert_lay1->addWidget(ww2);
	vert_lay1->addWidget(reset_btn);
	vert_lay1->addStretch(2);
	vert_lay1->setAlignment(titre,Qt::AlignHCenter);
	ww->setMaximumWidth(300);
	ww->setMinimumWidth(300);
	
// PLACEMENT DES ELEMENTSS
	lay->addWidget(fwidget);
	lay->addWidget(ww);;
	setCentralWidget(w);
	
// CONNEXION DES BOUTONS AUX FONCTIONS
	QObject::connect(next_btn, SIGNAL(clicked()), fwidget, SLOT(next()) );
	QObject::connect(timer, SIGNAL(timeout()),fwidget, SLOT(next()) );
	QObject::connect(play_btn, SIGNAL(clicked(bool)), this, SLOT(start_timer(bool)) );
	QObject::connect(pause_btn, SIGNAL(clicked(bool)), timer, SLOT(stop()) );

   
}

FireScreen::~FireScreen()
{
	delete(fwidget);
}

/***	Slots	***/
void FireScreen::start_timer(bool b)
{
	timer->start(delai);
}



