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
	QPainter* bufferPainter;
	int tailleCell;
	
	long temps;
	bool running;
	
	float probaMatriceReset; // TODO temp
	
public:
	// Constructeur et desctructeur
	FireWidget(int hauteur, int largeur, float proba = 0.60, float coef_brulure=1.0);
	virtual ~FireWidget();
	
	void newForet(int _hauteur, int _largeur, float _proba, float _coef_brulure);
	
	// Autres méthodes
	void setColor(int colorIndice);
	
// AFFICHAGES
	void drawCell(int width, int height);
	void drawTree(const Arbre* ab);

	void drawForest();
	void drawFire();
	
	bool eteindreFeu(int ligne, int colonne);
	bool allumerFeu(int ligne, int colonne);
	bool finirFeu(int ligne, int colonne);
	
	void reset();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
// 	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
public slots:
	bool next();
// 	void transition(long x);
};

#endif // FIREWIDGET_H
