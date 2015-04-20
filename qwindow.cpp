#include "qwindow.h"
#include <QtGui/QHBoxLayout>

void Qwindow::resizeEvent(QResizeEvent* e )
{
	QWidget::resizeEvent(e);
}

Qwindow::Qwindow(int _hauteur, int _largeur, float _proba, long _temps)
{
	this->terminal = new Qterminal(_hauteur,_largeur,_proba,_temps);
	
	// Conteneur général
    QWidget *w = new QWidget(this);
	
	QHBoxLayout *lay = new QHBoxLayout(w);
	lay->addWidget(terminal);
	
	setCentralWidget(w);
}

Qwindow::~Qwindow()
{

}

