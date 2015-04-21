#ifndef FIRESCREEN_H
#define FIRESCREEN_H

#include <QtGui/QMainWindow>

#include "firewidget.h"

class FireScreen : public QMainWindow
{
private:
	FireWidget* fwidget;
public:
	FireScreen();
};

#endif // FIRESCREEN_H
