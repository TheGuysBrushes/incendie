#include "fwelcome.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
/*
 * - BUG Les sliders/labels changent de dimension lorsqu'il y a un nombre à 2 chiffres après la virgule
 */

Fwelcome::Fwelcome(QWidget* parent): QDialog(parent)
{
	
	// Conteneur
	QVBoxLayout* lay = new QVBoxLayout(this);
	QWidget* ww = new QWidget(this);
	QWidget* www = new QWidget(this);
	QGridLayout* grid_lay = new QGridLayout(ww);
	QHBoxLayout* h_lay = new QHBoxLayout(www);
	
	// Initialisation des composants
	QString s = " Bienvenue sur l'automate de simulation de feux de foret. \n Veuillez renseigner les differents parametres pour la simulation \n que vous allez effectuer, puis validez. \n";
	QLabel* present = new QLabel(s);
	present->setWordWrap(true);
	
	QLabel* h_lbl = new QLabel("Hauteur : ");
	QLabel* l_lbl = new QLabel("Largeur : ");
	QLabel* p_lbl = new QLabel("Probabilite : ");
	QLabel* c_lbl = new QLabel("Coefficient : ");
	
	haut_spin = new QSpinBox(ww);
	haut_spin->setMinimum(100);
	haut_spin->setMaximum(1000);
	haut_spin->setSingleStep(25);
	haut_spin->setAccelerated(1);
	
	larg_spin = new QSpinBox(ww);
	larg_spin->setMinimum(100);
	larg_spin->setMaximum(1000);
	larg_spin->setSingleStep(25);
	larg_spin->setAccelerated(1);
	
	QSlider * slide_p = new QSlider(Qt::Horizontal,0);
	slide_p->setMaximum(100);
	slide_p->setMinimum(1);
	
	QSlider* slide_c = new QSlider(Qt::Horizontal,0);
	slide_c->setMaximum(100);
	slide_c->setMinimum(1);
	
	p_value = new QLabel();
	c_value = new QLabel();
	
	valid_btn = new QPushButton("Valider");
	valid_btn->setDefault(true);
	
	cancel_btn = new QPushButton("Annuler");
	cancel_btn->setVisible(false);
	grid_lay->addWidget(h_lbl,0,0);
	grid_lay->addWidget(haut_spin,0,1);
	
	grid_lay->addWidget(l_lbl,1,0);
	grid_lay->addWidget(larg_spin,1,1);
	
	grid_lay->addWidget(p_lbl,2,0);
	grid_lay->addWidget(slide_p,2,1);
	grid_lay->addWidget(p_value,2,2);
	
	grid_lay->addWidget(c_lbl,3,0);
	grid_lay->addWidget(slide_c,3,1);
	grid_lay->addWidget(c_value,3,2);
	
	h_lay->addWidget(valid_btn);
	h_lay->addWidget(cancel_btn);
	
	lay->addWidget(present);
	lay->addWidget(ww);
	lay->addWidget(www);
	
	
	// Connection entre Slider et Label associé
	connect(slide_p,	SIGNAL(valueChanged(int)), this, SLOT(set_proba(int)) );
	connect(slide_c,	SIGNAL(valueChanged(int)), this, SLOT(set_coef(int)) );
	connect(valid_btn, SIGNAL(clicked()), this, SLOT(accept()) );
	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(reject()) );
	
	// Initialisation des sliders
	slide_c->setValue(50);
	slide_p->setValue(50);
}

Fwelcome::~Fwelcome()
{

}

/*** SLOTS ***/
void Fwelcome::set_proba(int x)
{
	proba = (float) x/100;
	p_value->setText(QString::number(proba));
}

void Fwelcome::set_coef(int x)
{
	coef_brulure = (float) x/100;
	c_value->setText(QString::number(coef_brulure));
}


