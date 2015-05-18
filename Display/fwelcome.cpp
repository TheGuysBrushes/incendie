#include "fwelcome.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QApplication>
#include <qdesktopwidget.h>
/*
 * - BUG Ugo : Les sliders/labels changent de dimension lorsqu'il y a un nombre à 2 chiffres après la virgule
 */

using namespace std;

Fwelcome::Fwelcome(QWidget* parent): QDialog(parent)
{
	// Initialisation des composants dynamiques (SpinBox exceptées)
	p_value = new QLabel();
	c_value = new QLabel();
// 	directory= ""; TODO removeit
	
	PB_load= new QProgressBar();
	PB_load->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	PB_load->setFixedHeight(20);
	PB_load->setHidden(true);
	
	initComponents();
	
	file= new ifstream;
}

Fwelcome::~Fwelcome(){
	delete(haut_spin);
	delete(larg_spin);
	delete(p_value);
	delete(c_value);
// 	delete(valid_btn);
// 	delete(cancel_btn);
}

/*** Autres Méthodes ***/
void Fwelcome::initComponents(){

	/* Conteneurs */
	QVBoxLayout* lay = new QVBoxLayout(this);
	
	/* Initialisation des composants statiques (hors SpinBox ) */
	
	// TEXTE
	#if FRENCH
	QString s = " Bienvenue sur l'automate de simulation de feux de foret. Veuillez renseigner les differents parametres, puis validez.\n";
	#else
	QString s = "Welcome on the automaton of forest fire simulation. Please, enter parameters, then confirm\n";
	#endif
	
	QLabel* present = new QLabel(s);
	present->setWordWrap(true);
	present->setAlignment(Qt::AlignCenter);
	present->setMinimumSize(400, 60);
	
// REGLAGES
	QWidget* WSettings = new QWidget();
	
	QGridLayout* grid_lay = new QGridLayout(WSettings);
	
	// SPINBOXS
		// SpinBox largeur
		#if FRENCH
		QLabel* l_lbl = new QLabel("Largeur : ");
		#else
		QLabel* l_lbl = new QLabel("Width : ");
		#endif
		
		larg_spin = new QSpinBox(WSettings);
			larg_spin->setMinimum(100);
			larg_spin->setMaximum(QApplication::desktop()->screenGeometry().width() -250-15);
			larg_spin->setValue(450);
			larg_spin->setSingleStep(25);
			larg_spin->setAccelerated(1);
		
		// SpinBox Hauteur
		#if FRENCH
		QLabel* h_lbl = new QLabel("Hauteur : ");
		#else
		QLabel* h_lbl = new QLabel("Height : ");
		#endif
		
		haut_spin = new QSpinBox(WSettings);
			haut_spin->setMinimum(100);
			haut_spin->setMaximum(QApplication::desktop()->screenGeometry().height()-45-25 ); // 45 pixel à cause des marges et menu (observé 43)
			haut_spin->setValue(300);
			haut_spin->setSingleStep(25);
			haut_spin->setAccelerated(1);
	
	// SLIDERS
		// Probabilite
		#if FRENCH
		QLabel* p_lbl = new QLabel("Probabilite : ");
		#else
		QLabel* p_lbl = new QLabel("Probability : ");
		#endif
		
		QSlider * slide_p = new QSlider(Qt::Horizontal, 0);
		slide_p->setMaximum(100);
		slide_p->setMinimum(1);

		// Coefficient
	// 	#if FRENCH
	// 	#else
	// 	#endif
		QLabel* c_lbl = new QLabel("Coefficient : ");
		
		QSlider* slide_c = new QSlider(Qt::Horizontal, 0);
		slide_c->setMaximum(100);
		slide_c->setMinimum(1);
				
	/* Emboitements */
	grid_lay->addWidget(l_lbl, 0,0);
	grid_lay->addWidget(larg_spin, 0,1);
	
	grid_lay->addWidget(h_lbl, 1,0);
	grid_lay->addWidget(haut_spin, 1,1);
	
	grid_lay->addWidget(p_lbl,2,0);
	grid_lay->addWidget(slide_p,2,1);
	grid_lay->addWidget(p_value,2,2);
	
	grid_lay->addWidget(c_lbl,3,0);
	grid_lay->addWidget(slide_c,3,1);
	grid_lay->addWidget(c_value,3,2);
			
	
	// BOUTONS
	QWidget* WButtons= new QWidget();
		QHBoxLayout* h_lay = new QHBoxLayout(WButtons);
		
			#if FRENCH
			QPushButton* valid_btn = new QPushButton("Valider");
			cancel_btn = new QPushButton("Annuler");
			QPushButton* load_btn= new QPushButton("Charger une foret", this);
			#else 
			QPushButton* valid_btn = new QPushButton("Confirm");
			// TODO placement du bouton de chargemetn
			cancel_btn = new QPushButton("Cancel");
			QPushButton* load_btn= new QPushButton("Load forest", this);
			#endif
			
				valid_btn->setDefault(true);
				cancel_btn->setVisible(false);
			
		h_lay->addWidget(valid_btn);
		h_lay->addWidget(cancel_btn);

		
	lay->addWidget(present);
	lay->addWidget(WSettings);
	lay->addWidget(WButtons);
	lay->addWidget(PB_load);

	/* Connexion entre les différents éléments */
	connect(slide_p,	SIGNAL(valueChanged(int)), this, SLOT(set_proba(int)) );
	connect(slide_c,	SIGNAL(valueChanged(int)), this, SLOT(set_coef(int)) );
	
	connect(valid_btn,	SIGNAL(clicked()), this, SLOT(accept()) );
	connect(cancel_btn,	SIGNAL(clicked()), this, SLOT(reject()) );
	connect(load_btn,	SIGNAL(clicked()), this, SLOT(load()) );

	// Appel à setValue pour déclencer l'affichage de la valeur à la construction du widget
	slide_c->setValue(50);
	slide_p->setValue(50);
}

void Fwelcome::addCancel() const
{
	cancel_btn->setVisible(true);
}

#include <fstream>
#include <iostream>
void Fwelcome::load()
{
	std::string filename= "./Resources/foret1.dat";
	
// 	file= new ifstream(filename.c_str(), ios::in|ios::binary);
	
	file->open(filename.c_str(), ios::in|ios::binary);
	if (!file->is_open()){
		std::cout<< "Echec ouverture fichier de sauvegarde"<< std::endl;
	}
	else {
		int x; // x est utilisé pour la largeur ET la hauteur
		
		file->read( (char *)&(x), sizeof(int));
		larg_spin->setValue(x);
		#if DEBUG_LOAD
		cout<< "Taille : " << x<< " en largeur ";
		#endif
		
		file->read( (char *)&(x), sizeof(int));
		haut_spin->setValue(x);
		#if DEBUG_LOAD
		cout<<x<< " en hauteur" <<endl;
		#endif
		
// 		file->close();
// 		directory= filename;
		accept();
	}
}



/*** SLOTS ***/
/**
 * Met à jour la valeur de la probabilité qu'une cellule soit un arbre
 * et affiche la valeur courante.
 * @param int Probabilité qu'une cellule devienne un arbre.
 * @author Ugo
 */
void Fwelcome::set_proba(int x){
	proba = (float) x/100;
	p_value->setText(QString::number(proba, 'f', 2));
}

/**
 * Met à jour la valeur du coefficient de combustion de l'incendie
 * et affiche la valeur courante
 * @param int Coefficient de combustion
 * @author Ugo
 */
void Fwelcome::set_coef(int x){
	coef_brulure = (float) x/100;
	c_value->setText(QString::number(coef_brulure, 'f', 2));
}