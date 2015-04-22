#include "firescreen.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtCore/QString>
/*		BUG 
 *  - Lorsque le clic est enfoncé et que l'on sort du cadre du widget, Arrêt brutal 
 * 
 * 
 */

FireScreen::FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure/*, QWidget* parent, Qt::WindowFlags flags*/)
:	/*QMainWindow(parent, flags),*/	delai(nTemps)
{
// AFFICHEUR DE FORET
	fwidget= new FireWidget(hauteur, largeur, proba, coef_brulure);
	timer = new QTimer();
	nb_tour = 0;
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
   QWidget* www = new QWidget(ww2);
   QHBoxLayout* h_lay1 = new QHBoxLayout(www);
   
   // Element du panneau de direction de l'automate
   QLabel* titre = new QLabel("Automate cellulaire");
   QLabel* trans_p2p = new QLabel("Transmission pas-a-pas : ");
   QLabel* trans_con = new QLabel("Transmission continue : ");
   QPushButton* next_btn = new QPushButton("Next");
   QPushButton* play_btn = new QPushButton("Play");
   QPushButton* pause_btn = new QPushButton("Pause");
   QPushButton* reset_btn = new QPushButton("Reset ! Be careful");
   QLabel* tour_lbl = new QLabel("Nombre de tours :");
   cpt_debug = new QLabel();
   // Ajouter la scrollbar horizontale
   
	// Touches d'améliorations visuelles
	titre->setStyleSheet("color : darkblue; font : bold italic 20px;");
	trans_con->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	trans_p2p->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	tour_lbl->setStyleSheet("QLabel {  color : darkblue; }");
	cpt_debug->setStyleSheet("QLabel { color : darkblue; }");
	
	// Ajout des éléments dans les conteneurs
	grid_lay1->addWidget(next_btn,0,0);
	
	grid_lay2->addWidget(play_btn,0,0);
	grid_lay2->addWidget(pause_btn,0,1);
	
	h_lay1->addWidget(tour_lbl);
	h_lay1->addWidget(cpt_debug);
   
	vert_lay1->addWidget(titre);
	vert_lay1->addWidget(trans_p2p);
	vert_lay1->addWidget(ww1);
	vert_lay1->addWidget(trans_con);
	vert_lay1->addWidget(ww2);
	vert_lay1->addWidget(www);
	vert_lay1->addWidget(reset_btn);
	
	vert_lay1->addStretch(2);
	vert_lay1->setAlignment(titre,Qt::AlignHCenter);
	ww->setMaximumWidth(250);
	ww->setMinimumWidth(250);
	
// PLACEMENT DES ELEMENTSS
	lay->addWidget(fwidget);
	lay->addWidget(ww);;
	setCentralWidget(w);
	
// CONNEXION DES BOUTONS AUX FONCTIONS
	QObject::connect(next_btn,		SIGNAL(clicked()), fwidget,	SLOT(next()) );
	QObject::connect(timer,			SIGNAL(timeout()),fwidget, 	SLOT(next()) );
	QObject::connect(play_btn,		SIGNAL(clicked(bool)), this,	SLOT(start_timer(bool)) );
	QObject::connect(pause_btn,	SIGNAL(clicked(bool)), timer,	SLOT(stop()) );
	QObject::connect(timer, 		SIGNAL(timeout()), this,		SLOT(compteur()) );
	QObject::connect(next_btn, 	SIGNAL(clicked(bool)), this,	SLOT(compteur()) );
	QObject::connect(reset_btn, SIGNAL(clicked(bool)), this, SLOT(raz_matrice()) );

   
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

void FireScreen::compteur()
{
	nb_tour += 1;
	cpt_debug->setText(QString::number(nb_tour));
}

void FireScreen::raz_matrice()
{
}


