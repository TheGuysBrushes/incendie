#include "fwelcome.h"

// Composants Qt qui ne sont pas des attributs de classe

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtGui/QDesktopServices>

#include <QtCore/QUrl>
/*
 * TODO Commenter les fonctions de fwelcome
 */
#define FORESTPICTURESLOCATION "../Resources/Pictures"
 
using namespace std;

Fwelcome::Fwelcome(QWidget* parent): QDialog(parent), picturesBrowserLocation(QDir::toNativeSeparators(FORESTPICTURESLOCATION))
{
	createComponents();
	
	initComponents();
}

Fwelcome::Fwelcome(QWidget* parent, int _largeur, int _hauteur): QDialog(parent), picturesBrowserLocation("../Resources/Pictures")
{
	createComponents();
	
	initComponents();
	
	larg_spin->setValue(_largeur);
	haut_spin->setValue(_hauteur);
}


Fwelcome::~Fwelcome(){
	delete gridLayButtons;

	delete(haut_spin);
	delete(larg_spin);
	delete(p_value);
	delete(c_value);

    delete max_width_btn ;
    delete min_width_btn ;
    delete max_height_btn ;
    delete min_height_btn ;

	delete(restoreBtn);
	delete(cancel_btn);
	delete valid_btn;
	delete seedBtn;
	delete loadFromImgBtn;
	
	delete(fileDialog);
	
// 	delete pictureForest; WARNING fait planter
	delete file;
}

/*#####################*/
/*** Autres Méthodes ***/
/*#####################*/
void Fwelcome::createComponents()
{
	haut_spin= NULL;
	larg_spin= NULL;

	// Initialisation des composants dynamiques (SpinBox exceptées)
	p_value = new QLabel;
	c_value = new QLabel;
	
	pictureForest= new QImage;
	
	gridLayButtons = NULL;

    valid_btn = 	new QPushButton(tr("Confirm"));
    cancel_btn = 	new QPushButton(tr("Cancel"));
    loadFromImgBtn = new QPushButton(tr("Create from image"));
    restoreBtn= 	new QPushButton(tr("Load forest"));
    seedBtn = 	new QPushButton(tr("Load a seed"));

    max_width_btn = 	new QPushButton(tr("max"));
    min_width_btn = 	new QPushButton(tr("min");

    max_height_btn = 	new QPushButton(tr("max"));
    min_height_btn = 	new QPushButton(tr("min"));
	
	fileDialog = NULL;
	file= new ifstream;
}

void Fwelcome::initComponents(){
	cancel_btn->setVisible(false);

	/* Conteneurs */
	QVBoxLayout* lay = new QVBoxLayout(this);
	
	/* Initialisation des composants statiques (hors SpinBox ) */
	
	// TEXTE
    QString s = tr("Welcome on the automaton of forest fire simulation. Please, enter parameters, then confirm");
	
    QLabel* present = new QLabel(s + "\n");
	present->setWordWrap(true);
	present->setAlignment(Qt::AlignCenter);
	present->setMinimumSize(400, 60);
	
// REGLAGES
	QWidget* WSettings = new QWidget;
	
	QGridLayout* grid_lay_settings = new QGridLayout(WSettings);
	
	// SPINBOXS
		// SpinBox largeur
        QLabel* l_lbl = new QLabel(tr("Width : "));
		
		larg_spin = new QSpinBox(WSettings);
			larg_spin->setMinimum(100);
            larg_spin->setMaximum( Max_larg );
			larg_spin->setValue(450);
			larg_spin->setSingleStep(25);
			larg_spin->setAccelerated(1);
		
		// SpinBox Hauteur
        QLabel* h_lbl = new QLabel(tr("Height : "));
		
		haut_spin = new QSpinBox(WSettings);
			haut_spin->setMinimum(100);
            haut_spin->setMaximum( Max_haut );
			haut_spin->setValue(300);
			haut_spin->setSingleStep(25);
			haut_spin->setAccelerated(1);
	
	// SLIDERS
		// Probabilite
        QLabel* p_lbl = new QLabel(tr("Probability : "));

		QSlider * slide_p = new QSlider(Qt::Horizontal, 0);
		slide_p->setMaximum(100);
		slide_p->setMinimum(1);

		// Coefficient
        QLabel* c_lbl = new QLabel( tr("Coefficient : ") );
		
		QSlider* slide_c = new QSlider(Qt::Horizontal, 0);
		slide_c->setMaximum(100);
		slide_c->setMinimum(1);
				
    /* Emboitements */
    grid_lay_settings->setSpacing(1);
        p_value->setAlignment(Qt::AlignCenter);
        p_value->setFrameStyle(2);
        c_value->setAlignment(Qt::AlignCenter);
        c_value->setFrameStyle(2);

    grid_lay_settings->addWidget(l_lbl, 0,0);
    grid_lay_settings->addWidget(larg_spin, 0,1, 1,2);
    grid_lay_settings->addWidget(min_width_btn, 0,3);
    grid_lay_settings->addWidget(max_width_btn, 0,4);
//    grid_lay_settings->addLayout(width_btn_box, 0,3);

    grid_lay_settings->addWidget(h_lbl, 1,0);
    grid_lay_settings->addWidget(haut_spin, 1,1, 1,2);
    grid_lay_settings->addWidget(min_height_btn, 1,3);
    grid_lay_settings->addWidget(max_height_btn, 1,4);
//    grid_lay_settings->addLayout(height_btn_box, 1,3);

	grid_lay_settings->addWidget(p_lbl,2,0);
    grid_lay_settings->addWidget(slide_p,2,1, 1,3);
    grid_lay_settings->addWidget(p_value,2,4);
	
	grid_lay_settings->addWidget(c_lbl,3,0);
    grid_lay_settings->addWidget(slide_c,3,1, 1,3);
    grid_lay_settings->addWidget(c_value,3,4);
//    grid_lay_settings->setAlignment(p_value);
	
	// BOUTONS
	QWidget* WButtons= new QWidget;
		// TODO remettre dans le constructeur les création ?
		// Placement des boutons de la fenetre d'accueil, situés en bas de celle-ci
			// après avoir cliqué sur un des boutons, si une création de forêt est validé, la fenêtre se ferme
		gridLayButtons = new QGridLayout(WButtons);
		gridLayButtons->addWidget(valid_btn, 0,0, 1,0);
		gridLayButtons->addWidget(restoreBtn, 1,0);
		gridLayButtons->addWidget(loadFromImgBtn, 1,1);
		gridLayButtons->addWidget(seedBtn, 1, 2);

	lay->addWidget(present);
	lay->addWidget(WSettings);
	lay->addWidget(WButtons);

	/* Connexion entre les différents éléments */
	connect(slide_p,	SIGNAL(valueChanged(int)), this, SLOT(setProba(int)) );
	connect(slide_c,	SIGNAL(valueChanged(int)), this, SLOT(setCoef(int)) );
	
    connect(valid_btn,	SIGNAL(clicked()), this, SLOT(accept()) );
	connect(cancel_btn,	SIGNAL(clicked()), this, SLOT(reject()) );
	connect(restoreBtn,	SIGNAL(clicked()), this, SLOT(popSaveDialog()) );
	connect(loadFromImgBtn,	SIGNAL(clicked()), this, SLOT(popImageDIalog()) );
	connect(seedBtn,		SIGNAL(clicked()), this, SLOT(popSeedDialog()) );
    connect(min_width_btn,		SIGNAL(clicked()), SLOT(giveMinWidth()) );
    connect(max_width_btn,		SIGNAL(clicked()), SLOT(giveMaxWidth()) );
    connect(min_height_btn,		SIGNAL(clicked()), SLOT(giveMinHeight()) );
    connect(max_height_btn,		SIGNAL(clicked()), SLOT(giveMaxHeight()) );
//    connect(min_height_btn,		SIGNAL(clicked()), this,  );

	// Appel à setValue pour déclencer l'affichage de la valeur à la construction du widget
	slide_c->setValue(50);
	slide_p->setValue(50);
}

/*######################*/
void Fwelcome::addCancel() const
{
	gridLayButtons->removeWidget(valid_btn);
	gridLayButtons->addWidget(valid_btn, 0,0);
	gridLayButtons->addWidget(cancel_btn, 0,1);
	cancel_btn->setVisible(true);
}


/*################*/
/***	 Setters	 ***/
/*################*/

void Fwelcome::setProba(int x){
	proba = (float) x/100;
	p_value->setText(QString::number(proba, 'f', 2));
}

void Fwelcome::setCoef(int x){
	burningCoef = (float) x/100;
	c_value->setText(QString::number(burningCoef, 'f', 2));
}


/*######################*/
/***	 Chargements	 ***/
/*######################*/
void Fwelcome::openFile(QString filename)
{
	std::string filePath= filename.toStdString();
	
	file->open(filePath.c_str(), ios::in|ios::binary);
}

void Fwelcome::loadSizes()
{
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
}

/*######################*/

void Fwelcome::restore(QString filename)
{
	openFile(filename);
	
	if (!file->is_open()){
		std::cout<< "Echec ouverture fichier de sauvegarde"<< std::endl;
	}
	else {
		loadSizes();
		
		done(Restore);
	}
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

void Fwelcome::loadSeed(QString filename)
{
	openFile(filename);
	
	// Chargements des parametres dans le fichier
	file->read((char *)&(seed), sizeof(time_t));
	cout<< "graine : "<< seed<< endl;
	loadSizes();
	file->read( (char*)&(burningCoef), sizeof(float));
	
	file->close();
	done(RestoreSeed);
}


/*################*/
/***	 SLOTS	 ***/
/*################*/
void Fwelcome::popImageDIalog()
{
	fileDialog = new QFileDialog(this, "Chargement d'une Image");
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setDirectory(picturesBrowserLocation);
	QList<QUrl> urls= fileDialog->sidebarUrls();
//		urls << QUrl::fromLocalFile(QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation)))
        urls << QUrl::fromLocalFile(QDir::toNativeSeparators(FORESTPICTURESLOCATION));
    fileDialog->setSidebarUrls(urls);

	fileDialog->setNameFilter(tr("Images (*.png *.jpg *.jpeg *.tif *.tiff *.bmp)"));

	// Si l'utilisateur choisit un fichier
	if(fileDialog->exec()) {
		loadFromImg( fileDialog->selectedFiles()[0] );
		picturesBrowserLocation= fileDialog->directory();
	}
	//delete fileDialog;
}

void Fwelcome::popSaveDialog()
{
	fileDialog = new QFileDialog(this, "Chargement d'une Sauvegarde", "../Resources/");
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setNameFilter(tr("Sauvegarde de foret (*.data *.dat *.frt *.sav *.save)"));
	
	QString fileName;
	// Si l'utilisateur choisit un fichier
	if(fileDialog->exec()) {
		restore( fileDialog->selectedFiles()[0] );
	}
	
}

void Fwelcome::popSeedDialog()
{
	fileDialog = new QFileDialog(this, "Chargement d'une Graine",  "../Resources/");
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setNameFilter(tr("Sauvegarde de graine (*.seed)"));
	
	// Si l'utilisateur choisit un fichier
	if(fileDialog->exec()) {
		loadSeed( fileDialog->selectedFiles()[0] );
	}
}
