#include "windwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QSlider>
#include <QtGui/QPushButton>

using namespace std;

/*** Constructeur et destructeur ***/
/**
 * Constructeur de classe. Par défaut, la vitesse est 
 * initialisée à 5km/h.
 *@author Ugo 
 */
WindWidget::WindWidget():activeVar(true)
{
	// Initialisation des composants de la classe
	speed_lbl = new QLabel();
	speed_lbl->setFixedWidth(30);
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
	// QWidget conteneur des éléments concernant l'angle du vent
	QWidget* angle_container = new QWidget(this);

		QLabel* info_angle = new QLabel("Angle du vent : ");

		// Layout contenant les informations liées à l'angle
		QHBoxLayout* h_lay = new QHBoxLayout(angle_container);
		h_lay->addWidget(info_angle);
		h_lay->addWidget(angle_lbl);
		// QSlider permettant de faire varier l'angle du vent
		slider_angle = new QSlider(Qt::Horizontal);
			slider_angle->setMaximumWidth(180);
			slider_angle->setMinimum(0);
			slider_angle->setMaximum(360);

	// QWidget conteneur général des éléments de la vitesse du vent
	QWidget* speed_container = new QWidget(this);

		// Curseur vitesse
		QSlider* slider_vitesse = new QSlider(Qt::Vertical);
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
		QVBoxLayout* v_lay = new QVBoxLayout(speed_container);
		v_lay->addWidget(slider_vitesse);
		v_lay->addWidget(speed_label_container);
		
	QPushButton* varButton = new QPushButton("Varation Auto");

	// Conteneur général de l'ensemble des autres éléments
	QGridLayout* grid_lay = new QGridLayout(this);
	grid_lay->addWidget(wind, 0,0);
	grid_lay->addWidget(speed_container, 0,1, 1,1);
	grid_lay->addWidget(slider_angle, 1,0);
	grid_lay->addWidget(varButton,1,1);
	grid_lay->addWidget(angle_container, 2,0);	
	
	slider_angle->setValue(45);
	
	// Connexion des sliders avec les setters de la classe :
	// 	permet de gérer dynamiquement les changements de valeurs
	connect(slider_angle, SIGNAL(valueChanged(int)),		this, SLOT(majAngle(int)) );
	connect(slider_vitesse, SIGNAL(valueChanged(int)),	this, SLOT(majSpeed(int)));
	connect(varButton, SIGNAL(clicked(bool)), this, SLOT(startTimer(bool)));
	connect(timer, SIGNAL(timeout()),this,SLOT(varWind()));
	slider_vitesse->setValue(2);

	// Appel à la fonction setAngle pour que l'initialisation des composants se fasse parfaitement
	setAngle(45);

}

void WindWidget::setAngle(int angle)
{
	angle_lbl->setText(QString::number(angle) + " degres");
	wind->setAngle(angle+270);
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
 * Met à jour la valeur de l'angle, l'affiche et
 * dessine la ligne directionnelle
 * @author Ugo
 */
void WindWidget::majAngle(int alpha)
{
	#if DEBUG_VENT
	cout << "angle de windwidget par appel de majAngle" << alpha << endl;
	#endif
	setAngle(alpha);
	wind->drawDir();
	update();
}

/**
 * Met à jour la valeur de la vitesse choisie et l'affiche
 * @author Ugo
 */
void WindWidget::majSpeed(int vitesse){
	
	speed_lbl->setText(QString::number(vitesse));
	speed = vitesse;
	emit modif_value(wind->getAngle(), speed);
}

/**
 * Slot déclenchant ou stoppant l'activation du timer gérant
 * la variation de l'angle du vent
 * @author Ugo
 */
void WindWidget::startTimer(bool )
{
	// On modifie la valeur de activeVar et on effectue l'action correspondante au nouvel état
	if(activeVar){
		timer->start(500);
		activeVar = false;
	}else{
		timer->stop();
		activeVar = true;
	}
}

/** 
 * Slot exécuté à chaque timeout du timer pour
 * faire varier la valeur de l'angle de facon aléatoire
 * @author Ugo
 * 
 */
void WindWidget::varWind()
{
	int coef = rand()%11 - 5;
	int angle = wind->getAngle()*(1.0+(coef/100.0));
	if(angle > 630)
		angle = 630;
	if(angle < 270)
		angle = 270;
	#if DEBUG_VAR
	cout << "angle de wind : " << wind->getAngle() << " ; " << endl;
	cout << "coefficient de variation " << coef << " ; " << endl;
	cout << "angle après modif " << angle << endl;
	#endif
	slider_angle->setValue(angle-270);
}
