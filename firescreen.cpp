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
 * 
 * 
 */

FireScreen::FireScreen(int hauteur, int largeur, float proba, long nTemps, float coef_brulure/*, QWidget* parent, Qt::WindowFlags flags*/)
:	/*QMainWindow(parent, flags),*/	delai(nTemps)
{
	QAction* exit = new QAction(this);
	exit->setText( "Quitter" );
	
	QAction* save = new QAction(this);
	save->setText( "Save" );
	
	menuBar()/*->addMenu( "Menu" )*/->addAction(exit);
	menuBar()->addAction(save);
	connect(exit, SIGNAL(triggered()), SLOT(close()) );
	
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
	next_btn = new QPushButton("Next");
	play_btn = new QPushButton("Play");
	pause_btn = new QPushButton("Pause");
	QPushButton* reset_btn = new QPushButton("Reset ! Be careful");
	QLabel* tour_lbl = new QLabel("Nombre de tours :");
	cpt_lbl = new QLabel();
	
	int val_init= nTemps;
	delai_lbl = new QLabel(QString::number(val_init));
	// Ajouter la scrollbar horizontale
	QSlider* slider = new QSlider(Qt::Horizontal,0);
	slider->setMinimum(10);
	slider->setMaximum(500);
	slider->setValue(val_init);
	
	// Touches d'améliorations visuelles et d'initialisation de propriétés
	titre->setStyleSheet("color : darkblue; font : bold italic 20px;");
	trans_con->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	trans_p2p->setStyleSheet("text-decoration : underline; color : darkblue ; font : italic 14px");
	tour_lbl->setStyleSheet("QLabel {  color : darkblue; }");
	cpt_lbl->setStyleSheet("QLabel { color : darkblue; }");
	delai_lbl->setStyleSheet("QLabel { color : darkblue; }");
	
	pause_btn->setDisabled(true);
	
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
	ww->setMaximumWidth(250);
	ww->setMinimumWidth(250);
	
// PLACEMENT DES ELEMENTSS
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
	QObject::connect(next_btn, 	SIGNAL(clicked(bool)), this,	SLOT(compteur()) );

	/// @author Florian
	QObject::connect(reset_btn, SIGNAL(clicked()), fwidget, SLOT(restart()) );
	QObject::connect(reset_btn, SIGNAL(clicked(bool)),this,SLOT(init_btn()) );
		
	// Tests pour RAZ de la matrice
// 	QObject::connect(reset_btn, SIGNAL(clicked()), this, SLOT(raz_matrice()) );
// 	QObject::connect(this, SIGNAL(ask_restart()), fwidget, SLOT(restart()));
	QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(set_delai(int )));
	
	connect(fwidget, SIGNAL(SizeCellChanged(int,int)), this, SLOT(minimumSize(int,int)));
}

FireScreen::~FireScreen()
{
	delete(fwidget);
}

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
	pause_btn->setEnabled(true);
	play_btn->setEnabled(false);
	next_btn->setEnabled(false);
}

void FireScreen::stop_timer(bool )
{
	timer->stop();
	pause_btn->setEnabled(false);
	next_btn->setEnabled(true);
	play_btn->setEnabled(true);
}

void FireScreen::set_delai(int x)
{
	delai = (long)x;
	delai_lbl->setText(QString::number(x));
	timer->setInterval(delai);
}

void FireScreen::init_btn()
{
	play_btn->setEnabled(true);
	next_btn->setEnabled(true);
	pause_btn->setDisabled(true);
	timer->stop();
}


void FireScreen::compteur()
{
	// IMPROVEIT tour suivant fait dans le compteur pour l'instant
	if (fwidget->next()){
		nb_tour += 1;
		cpt_lbl->setText(QString::number(nb_tour));
	}
	else timer->stop();
}

void FireScreen::minimumSize(int largeur, int hauteur)
{
	setMinimumSize(largeur+250, hauteur);
}


