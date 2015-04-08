#include "AutomateFeu.h"

#include <QTimer>
#include <iostream>

AutomateFeu::AutomateFeu()
{
    QTimer* timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), SLOT(output()) );
    timer->start( 1000 );
}

AutomateFeu::~AutomateFeu()
{}

void AutomateFeu::output()
{
    std::cout << "Hello World!" << std::endl;
}

#include "AutomateFeu.moc"
