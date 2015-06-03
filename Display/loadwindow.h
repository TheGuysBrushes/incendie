#ifndef LOADWINDOW_H
#define LOADWINDOW_H


#include <QtGui/QProgressBar>
#include <QtGui/QWidget>

#include "../Moteur/loadprogress.h"

class LoadWindow : public LoadProgress, QWidget
{
private:
	QProgressBar* PB_load;
	
public:
	LoadWindow();
	virtual ~LoadWindow();
	
	virtual void setProgress(int pourcentage);
	virtual void closeProgress();
};

#endif // LOADWINDOW_H
