#include "windwidget.h"
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QString>
#include <QtGui/QSlider>

/*** Constructeur et destructeur ***/
WindWidget::WindWidget():vitesse(5){
	
	QGridLayout* grid_lay = new QGridLayout(this);
	QWidget* w = new QWidget(this);
	QWidget* ww = new QWidget(this);
	QWidget* www = new QWidget(this);	
	
	QHBoxLayout* h_lay = new QHBoxLayout(w);
	QVBoxLayout* v_lay = new QVBoxLayout(ww);
	QHBoxLayout* h_lay2 = new QHBoxLayout(www);
	
	QSlider* slider_angle = new QSlider(Qt::Horizontal);
	slider_angle->setMinimum(0);
	slider_angle->setMaximum(360);
	slider_angle->setValue(45);	// position initiale du slider
	
	QSlider* slider_vitesse = new QSlider(Qt::Vertical);
	slider_vitesse->setMinimum(5);
	slider_vitesse->setMaximum(130);
	slider_vitesse->setValue(vitesse);
	
	QLabel* info_vitesse = new QLabel(" km/h");
	vitesse_lbl = new QLabel(QString::number(vitesse));
	
	
	wind = new WindCircle(45);
	
	QLabel* info_angle = new QLabel("Angle du vent : ");
	angle_lbl = new QLabel(QString::number(wind->getAngle())+" degres");
	
	h_lay->addWidget(info_angle);
	h_lay->addWidget(angle_lbl);
	
	h_lay2->addWidget(vitesse_lbl);
	h_lay2->addWidget(info_vitesse);
	
	v_lay->addWidget(slider_vitesse);
	v_lay->addWidget(www);
	
	grid_lay->addWidget(wind,0,0);
	grid_lay->addWidget(ww,0,1,1,1);
	grid_lay->addWidget(slider_angle,1,0);
	grid_lay->addWidget(w,2,0);
	
	connect(slider_angle, SIGNAL(valueChanged(int)), this, SLOT(set_angle(int)) );
	connect(slider_vitesse, SIGNAL(valueChanged(int)), this, SLOT(set_vitesse(int)));
	
}

WindWidget::~WindWidget(){

}

/*** Autres Méthodes ***/



/*** Events ***/
void WindWidget::resizeEvent(QResizeEvent* Qevent)
{
}

/*** Slots ***/
void WindWidget::set_angle(int x){

	angle_lbl->setText(QString::number(x)+" degres");
	wind->setAngle(x);
	wind->drawDir();
	update();
	emit modif_value(x);
}

void WindWidget::set_vitesse(int y){
	
	vitesse_lbl->setText(QString::number(y));
	vitesse = y;
	emit modif_value(y);
}

