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
	int tailleCell;
	
	
public:
	// Constructeur et desctructeur
	FireWidget(int hauteur, int largeur, float proba = 0.60,float coef_brulure=1.0);
	virtual ~FireWidget();
	
	// Setters
	void setColor(int colorIndice);
	
// AFFICHAGES
	void drawVariable(int posWidth, int posHeight, const Cellule* cell);
	
	void drawForest();
	void drawFire();
// 	void drawForest(void(*pDraw)(int, int, const Cellule*) );
// 	void drawForest(void* drawCell(int, int, const Cellule*));
// 	void drawForest(std::function<void(void)> drawCell );

protected:
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
public slots:
	void next();
};

#endif // FIREWIDGET_H
