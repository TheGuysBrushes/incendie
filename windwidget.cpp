#include "windwidget.h"
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QString>
#include <QtGui/QSlider>


/*** Constructeur et destructeur ***/
/**
 * Constructeur de classe. Par défaut, la vitesse est 
 * initialisée à 5km/h.
 *@author Ugo 
 */
WindWidget::WindWidget()
{
	// Initialisation des composants de la classe (TODO ajouter menus ?)
	vitesse_lbl = new QLabel();
	wind = new WindCircle(45);	// creer constructeur pour pouvoir initialiser sans parametre à vide et utiliser le connect du slider pour initialiser
	angle_lbl = new QLabel(/* QString::number(wind->getAngle()).append("°")*/ );
	
	initComponents();
}

WindWidget::~WindWidget(){
	delete(wind);
	delete(angle_lbl);
	delete(vitesse_lbl);
}

/*** Composants ***/
/**
 * Initialise les composants graĥiques statiques
 * 	et organise la hiérarchie des différents composants
 * @author Ugo
 */
void WindWidget::initComponents()
{
	
	/// REMOVEIT J'ai mis des tabulation pour mieux voir quels widgets/éléments sont à l'intérieur des autres
	///				On pourrait mettre des accolades mais ca alourdirai le code ?
	
	// TODO comment :  ajout "nom composant" (position x,y) et renommer w explicitement
	QWidget* w1 = new QWidget(this);

		// TODO comment : ajout "nom composant"
		QHBoxLayout* h_lay = new QHBoxLayout(w1);
	
			// TODO comment ??
			QLabel* info_angle = new QLabel("Angle du vent : ");
		
		// TODO comment ??
		h_lay->addWidget(info_angle);
		h_lay->addWidget(angle_lbl);
	
	
	// Curseur angle (position x,y)
	QSlider* slider_angle = new QSlider(Qt::Horizontal);
		slider_angle->setMinimum(0);
		slider_angle->setMaximum(360);
	
	
	// TODO comment : ajout "nom composant" (position x,y) et renommer ww explicitement
	QWidget* w2 = new QWidget(this);
	
		// TODO comment : ajout "nom composant"
		QVBoxLayout* v_lay = new QVBoxLayout(w2);
	
		//ELEMENTS DE v_lay
		// Curseur vitesse
		QSlider* slider_vitesse = new QSlider(Qt::Vertical);
		slider_vitesse->setMinimum(5);
		slider_vitesse->setMaximum(130);
// 		slider_vitesse->setValue(vitesse);
		
		// TODO comment : ajout "nom composant" et renommer www  explicitement
		QWidget* ww = new QWidget();

			// TODO comment : ajout "nom composant"
			QHBoxLayout* h_lay2 = new QHBoxLayout(ww);
				QLabel* unit_vitesse = new QLabel(" km/h");

			// TODO comment ??
			h_lay2->addWidget(unit_vitesse);
			h_lay2->addWidget(vitesse_lbl);
		
		// TODO comment ??
		v_lay->addWidget(slider_vitesse);
		v_lay->addWidget(ww);
	
// TODO comment
	QGridLayout* grid_lay = new QGridLayout(this);
	
	// TODO comment ??
	grid_lay->addWidget(wind, 0,0);
	grid_lay->addWidget(w2, 0,1, 1,1);
	grid_lay->addWidget(slider_angle, 1,0);
	grid_lay->addWidget(w1, 2,0);
	
	
	// Connexion des sliders avec les setters de la classe :
	// 	permet de gérer dynamiquement les changements de valeurs
	connect(slider_angle, SIGNAL(valueChanged(int)), this, SLOT(set_angle(int)) );
	connect(slider_vitesse, SIGNAL(valueChanged(int)), this, SLOT(set_vitesse(int)));
	
	slider_vitesse->setValue(10);	// Attention si on met la valeur minimale(5), il n'y a pas de changement de valeur donc le label n'est pas mis à jour ?
	slider_angle->setValue(45);	// position initiale du slider IMPROVEIT est redondant avec le constructeur
// TODO enlever code en dur
}


/*** Events ***/
void WindWidget::resizeEvent(QResizeEvent* Qevent){
}


/*** Slots ***/
/**
 * Met à jour la valeur de l'angle, l'affiche et
 * dessine la ligne directionnelle
 * @author Ugo
 */
void WindWidget::set_angle(int x){

	angle_lbl->setText(QString::number(x)+" degres");
	wind->setAngle(x);
	wind->drawDir();
	update();
	emit modif_value(x);
}

/**
 * Met à jour la valeur de la vitesse choisie et l'affiche
 * @author Ugo
 */
void WindWidget::set_vitesse(int y){
	
	vitesse_lbl->setText(QString::number(y));
	vitesse = y;
	emit modif_value(y);
}