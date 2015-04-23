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
	Foret* foret;
// 	std::function<void> test;
	QImage* buffer;
	QColor* color;
	int tailleCell;
	
	long temps;
	bool running;
	
	float probaMatriceReset; // TODO temp
	
public:
	// Constructeur et desctructeur
	FireWidget(int hauteur, int largeur, float proba = 0.60, float coef_brulure=1.0);
	virtual ~FireWidget();
	
	void newForet(int _hauteur, int _largeur, float _proba = 0.60, float _coef_brulure=1.0);
	
	// Autres méthodes
	void setColor(int colorIndice);
	
// AFFICHAGES
// 	void drawVariable(int posWidth, int posHeight, const Cellule* cell);
	
	void drawForest();
	void drawFire();
// 	void drawForest(void(*pDraw)(int, int, const Cellule*) );
// 	void drawForest(void* drawCell(int, int, const Cellule*));
// 	void drawForest(std::function<void(void)> drawCell );
	
	bool eteindreFeu(int ligne, int colonne);
	bool allumerFeu(int ligne, int colonne);
	bool finirFeu(int ligne, int colonne);
	
	bool next();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
// 	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
public slots:
	void restart();
	
// 	void transition(long x);
};

#endif // FIREWIDGET_H
