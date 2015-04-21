#ifndef QWINDOW_H
#define QWINDOW_H

#include <QMainWindow>
#include "qterminal.h"


class Qwindow : public QMainWindow
{
Q_OBJECT
private:
	Qterminal* terminal;
public:
	Qwindow();
	virtual ~Qwindow();
};

#endif // QWINDOW_H
