#include "firescreen.h"

#include <QtGui/QHBoxLayout>

FireScreen::FireScreen()
{
   QWidget* w = new QWidget(this);
   QHBoxLayout* lay = new QHBoxLayout(w);
   
   this->fwidget = new FireWidget(40,40,0.6,150000);
   lay->addWidget(fwidget);
   
   setCentralWidget(w);
   
}


