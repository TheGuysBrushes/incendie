#include "fwelcome.h"

// Composants Qt qui ne sont pas des attributs de classe
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
/*
 * TODO Commenter les fonctions de fwelcome
 */

using namespace std;

Fwelcome::Fwelcome(QWidget* parent): QDialog(parent)
{
	createComponents();
	file= new ifstream;
	fileDialog = NULL;
	initComponents();
}

Fwelcome::Fwelcome(QWidget* parent, int _largeur, int _hauteur): QDialog(parent)
{
	createComponents();
	file= new ifstream;
	fileDialog = NULL;
	
	initComponents();
	
	larg_spin->setValue(_largeur);
	haut_spin->setValue(_hauteur);
}


Fwelcome::~Fwelcome(){
	delete(haut_spin);
	delete(larg_spin);
	delete(p_value);
	delete(c_value);
	delete(restoreBtn);
	delete(cancel_btn);
	delete(fileDialog);
}

/*** Autres Méthodes ***/
/**
 * Initialise les composants graphiques attributs de la classe
 * @author Ugo et Florian
 */
void Fwelcome::createComponents()
{
	// Initialisation des composants dynamiques (SpinBox exceptées)
	p_value = new QLabel();	
	c_value = new QLabel();
	
	pictureForest= new QImage();
	
	#if FRENCH
	valid_btn = new QPushButton("Valider");
	cancel_btn = new QPushButton("Annuler");
	QPushButton* load_btn= new QPushButton("Charger une foret", this);
	#else 
	valid_btn = new QPushButton("Confirm");
	cancel_btn = new QPushButton("Cancel");
	restoreBtn= new QPushButton("Load forest");
	loadFromImgBtn = new QPushButton("Create from image");
	#endif
	
	cancel_btn->setVisible(false);
}

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
		
		int largeurMaximaleMenusDroite= 300;
		
		larg_spin = new QSpinBox(WSettings);
			larg_spin->setMinimum(100);
			larg_spin->setMaximum( (QApplication::desktop()->screenGeometry().width() -largeurMaximaleMenusDroite -40)*1.5);
			larg_spin->setValue(450);
			larg_spin->setSingleStep(25);
			larg_spin->setAccelerated(1);
		
		// SpinBox Hauteur
		#if FRENCH
		QLabel* h_lbl = new QLabel("Hauteur : ");
		#else
		QLabel* h_lbl = new QLabel("Height : ");
		#endif
		
		int hauteurMaximaleBarresFenetre= 45;
		
		haut_spin = new QSpinBox(WSettings);
			haut_spin->setMinimum(100);
			haut_spin->setMaximum( (QApplication::desktop()->screenGeometry().height() -hauteurMaximaleBarresFenetre -25)*1.5); // 45 pixel à cause des marges et menu (observé 43)
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
		// TODO remettre dans le constructeur les création ?
		gridLay = new QGridLayout(WButtons);			
		gridLay->addWidget(valid_btn, 0,0, 1,0);
		gridLay->addWidget(restoreBtn, 1,0);
		gridLay->addWidget(loadFromImgBtn, 1,1);

	lay->addWidget(present);
	lay->addWidget(WSettings);
	lay->addWidget(WButtons);

	/* Connexion entre les différents éléments */
	connect(slide_p,	SIGNAL(valueChanged(int)), this, SLOT(set_proba(int)) );
	connect(slide_c,	SIGNAL(valueChanged(int)), this, SLOT(set_coef(int)) );
	
	connect(valid_btn,	SIGNAL(clicked()), this, SLOT(accept()) );
	connect(cancel_btn,	SIGNAL(clicked()), this, SLOT(reject()) );
	connect(restoreBtn,	SIGNAL(clicked()), this, SLOT(popSaveDialog()) );
	connect(loadFromImgBtn,	SIGNAL(clicked()), this, SLOT(popImageDIalog()) );

	// Appel à setValue pour déclencer l'affichage de la valeur à la construction du widget
	slide_c->setValue(50);
	slide_p->setValue(50);
}

void Fwelcome::addCancel() const
{
	gridLay->removeWidget(valid_btn);
	gridLay->addWidget(valid_btn, 0,0);
	gridLay->addWidget(cancel_btn, 0,1);
	cancel_btn->setVisible(true);
}

void Fwelcome::restore(QString filename)
{
	std::string filePath= filename.toStdString();
	
// 	file= new ifstream(filename.c_str(), ios::in|ios::binary);
	
	file->open(filePath.c_str(), ios::in|ios::binary);
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
		
		done(Restore);
	}
}

/**
 * Slot déclenché lors du clic sur le bouton d'importation d'image.
 * Ouvre une fenetre de selection d'un fichier image et appelle
 * la fonction de chargement d'image.
 * @author Ugo
 */
void Fwelcome::popImageDIalog()
{
	QString fileName;

	fileDialog = new QFileDialog(this);
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setNameFilter(tr("Images (*.png *.jpeg *.tif)"));
	if(fileDialog->exec())
		fileName = fileDialog->selectedFiles().at(0);
	else
		fileName = "../foret_pay.png";
			
	loadFromImg(fileName);
		
}

/**
 * Slot déclenché lors du clic surle bouton de chargement d'une sauvegarde.
 * Ouvre une fenetre de sélection d'un fichier de sauvegarde et appelle 
 * la fonction de chargement correspondant.
 * @author Ugo
 */
void Fwelcome::popSaveDialog()
{
	QString fileName;

	fileDialog = new QFileDialog(this);
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setNameFilter(tr("Sauvegarde (*.data *.frt *.dat *.sav)"));
	if(fileDialog->exec())
		fileName = fileDialog->selectedFiles().at(0);
	else
		fileName = "./Resources/foret1.dat";
			
	restore(fileName);

}


void Fwelcome::loadFromImg(QString filename)
{
	delete pictureForest;
	pictureForest= new QImage();
	pictureForest->load(filename);
	
	if (pictureForest->isNull()){
		std::cout<< "Echec ouverture fichier de sauvegarde"<< std::endl;
	}
	else {
		larg_spin->setValue(pictureForest->width());
		#if DEBUG_LOAD
		cout<< "Taille : " << pictureForest->width()<< " en largeur ";
		#endif
		
		haut_spin->setValue(pictureForest->height());
		#if DEBUG_LOAD
		cout<< pictureForest->height()<< " en hauteur" <<endl;
		#endif
		
		done(Load);
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