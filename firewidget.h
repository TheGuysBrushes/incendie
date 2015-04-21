#ifndef FIREWIDGET_H
#define FIREWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>

#include <functional>
#include <vector>

#include "foret.h"

class FireWidget : public QWidget {
Q_OBJECT
private:
	Foret foret;
// 	std::function<void> test;
	QImage* buffer;
	QColor* color;
	int cell_larg;
	int cell_haut;
	
	long temps;
	bool running;
	
public:
	// Constructeur et desctructeur
	FireWidget(int _hauteur, int _largeur, float _proba = 0.60, long _temps=150000);
	virtual ~FireWidget();
	
	// Autres méthodes
	void setColor(int colorIndice);
	
// AFFICHAGES
	void drawVariable(int posWidth, int posHeight, const Cellule* cell);
	
	void drawForest();
	void drawFire();
// 	void drawForest(void(*pDraw)(int, int, const Cellule*) );
// 	void drawForest(void* drawCell(int, int, const Cellule*));
// 	void drawForest(std::function<void(void)> drawCell );

protected:
// 	void mousePressEvent(QMouseEvent *event);
// 	void mouseMoveEvent(QMouseEvent *event);
// 	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
public slots:
	void next();
	void run();
	void pause();
	
// 	void transition(long x);
};

#endif // FIREWIDGET_H
