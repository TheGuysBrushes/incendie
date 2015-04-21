#include "firescreen.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

FireScreen::FireScreen()
{
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
   titre->setStyleSheet("color : darkblue; font : bold italic;");

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
	
   this->fwidget = new FireWidget(40,40,0.6,150000);
   lay->addWidget(fwidget);
   lay->addWidget(ww);;
   setCentralWidget(w);
   
}


