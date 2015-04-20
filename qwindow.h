#ifndef QWINDOW_H
#define QWINDOW_H

#include <QMainWindow>
#include "qterminal.h"


class Qwindow : public QMainWindow
{
private:
	Qterminal* terminal;
	
protected:
	virtual void resizeEvent(QResizeEvent* e);

public:
	Qwindow(int _hauteur, int _largeur, float _proba, long _temps);
	virtual ~Qwindow();
};

#endif // QWINDOW_H
