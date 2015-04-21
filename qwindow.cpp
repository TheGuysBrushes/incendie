#include "qwindow.h"

#include <QtGui/QHBoxLayout>

Qwindow::Qwindow()
{
   QWidget* w = new QWidget(this);
   QHBoxLayout* lay = new QHBoxLayout(w);
   
   this->terminal = new Qterminal(40,40,0.6,150000);
   lay->addWidget(terminal);
   
   setCentralWidget(w);
}

Qwindow::~Qwindow()
{

}

