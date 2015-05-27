#include "windwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QSlider>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

using namespace std;

/*** Constructeur et destructeur ***/
/**
 * Constructeur de classe. Par défaut, la vitesse est 
 * initialisée à 5km/h.
 *@author Ugo 
 */
WindWidget::WindWidget()
{
	// Initialisation des composants de la classe
	speed_lbl = new QLabel();
	speed_lbl->setFixedWidth(25);
	wind = new WindCircle();
	angle_lbl = new QLabel();
	angle_lbl->setFixedWidth(100);
	
	this->setMaximumHeight(300);
	
	timer = new QTimer();
	
	initComponents();
}

WindWidget::~WindWidget(){
	delete(wind);
	delete(angle_lbl);
	delete(speed_lbl);
}

/*** Composants ***/
/**
 * Initialise les composants graĥiques statiques
 * 	et organise la hiérarchie des différents composants
 * @author Ugo
 */
void WindWidget::initComponents()
{


	// QWidget conteneur général des éléments de la vitesse du vent
	QWidget* speed_container = new QWidget(this);

		// Curseur vitesse
		QSlider* slider_vitesse = new QSlider(Qt::Horizontal);
		slider_vitesse->setMinimum(1);
		slider_vitesse->setMaximum(100);


		// QWidget conteneur des éléments d'informations sur la vitesse du vent
		QWidget* speed_label_container = new QWidget();
			QLabel* unit_vitesse = new QLabel("km/h");

			// Layout contenant les labels sur la vitesse du vent
			QHBoxLayout* h_lay2 = new QHBoxLayout(speed_label_container);
			h_lay2->addWidget(speed_lbl);
			h_lay2->addWidget(unit_vitesse);

		// Layout vertical contenant le QSlider vitesse et le QWidget d'information 
		QHBoxLayout* v_lay = new QHBoxLayout(speed_container);
		v_lay->addWidget(slider_vitesse);
		v_lay->addWidget(speed_label_container);
		
		// TODO créer dans constructeur?
	/*QCheckBox* */varAngleBox = new QCheckBox("Variation Auto");
	varAngleBox->setChecked(false);

	// Conteneur général de l'ensemble des autres éléments
	QVBoxLayout* grid_lay = new QVBoxLayout(this);
	QWidget* container = new QWidget();
	QHBoxLayout* testLay = new QHBoxLayout(container);
	testLay->addWidget(speed_container);
	testLay->addWidget(varAngleBox);
	/*
	grid_lay->addWidget(wind, 0,0,1,1);
	grid_lay->addWidget(speed_container, 1,0);
	grid_lay->addWidget(varAngleBox,1,1);	
	*/
	grid_lay->addWidget(wind);
	grid_lay->addWidget(container);
	// Connexion des sliders avec les setters de la classe :
	// 	permet de gérer dynamiquement les changements de valeurs;
	connect(slider_vitesse,	SIGNAL(valueChanged(int)),		this, SLOT(majSpeed(int)));
// 	connect(varAngleBox, SIGNAL(stateChanged(int)), this, SLOT(switchAngleBox(int)));
	connect(wind, SIGNAL(modifAngle()), this, SLOT(majAngle()));
// 	connect(timer,	SIGNAL(timeout()),	this, SLOT(varWind()));
	slider_vitesse->setValue(2);

	// Appel à la fonction setAngle pour que l'initialisation des composants se fasse parfaitement
	setAngle(45);

}

/**
 * Envoie le signal de changement d'angle à firescreen
 * 
 */
void WindWidget::setAngle(int angle)
{
	#if DEBUG_VENT
	cout << "angle de windwidget envoyé a windcircle" << angle+270 << endl;
	#endif
	emit modif_value(wind->getAngle(), speed);
}


/*** Events ***/
void WindWidget::resizeEvent(QResizeEvent* Qevent)
{}


/*** Slots ***/
/**
 * Signale à firescreen que le vent a été modifié
 * @author Ugo
 */
void WindWidget::majAngle()
{
	#if DEBUG_VENT
// 	cout << "angle de windwidget par appel de majAngle" << alpha << endl;
	#endif
	emit modif_value(wind->getAngle(), speed);
}

/**
 * Met à jour la valeur de la vitesse choisie et l'affiche
 *  signale à firescreen que le vent a été modifié
 * @author Ugo
 */
void WindWidget::majSpeed(int vitesse){
	
	speed_lbl->setText(QString::number(vitesse));
	speed = vitesse;
	emit modif_value(wind->getAngle(), speed);
}


/** 
 * Slot exécuté à chaque timeout du timer pour
 * faire varier la valeur de l'angle de facon aléatoire
 * @author Ugo
 */
void WindWidget::changeWindDir()
{
	int variation= 2;
	if (varAngleBox->isChecked()){
		float coef = rand()%(2*variation +1) - variation;
		int angle = wind->getAngle()*(1.0+(coef/100.0));
		
		if(angle > 630)
			angle -= 360;
		if(angle < 270)
			angle += 360;
		#if DEBUG_VAR
		cout << "angle de wind : " << wind->getAngle() << " ; " << endl;
		cout << "coefficient de variation " << coef << " ; " << endl;
		cout << "angle après modif " << angle << endl;
		#endif
		wind->setAngle(angle);
		
		majAngle();
	}
}
