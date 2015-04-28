#ifndef FIREWIDGET_H
#define FIREWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <functional>
#include <vector>

#include "Moteur/foret.h"
#include "fwelcome.h"

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
	
public:
	// Constructeur et desctructeur
	FireWidget(int _largeur, int _hauteur, float proba = 0.60, float coef_brulure=1.0);
	virtual ~FireWidget();
	
	void newForet(int _largeur, int _hauteur, float _proba, float _coef_brulure);
	
	// Autres méthodes
	void setColor(int colorIndice);
	
// AFFICHAGES
	void drawCell(int colonne, int ligne);
	void drawTree(const Arbre* ab);

	void drawForest();
	void drawFire();
	
	bool eteindreFeu(int colonne, int ligne);
	bool allumerFeu(int colonne, int ligne);
	bool finirFeu(int colonne, int ligne);
	
	void set(int _larg, int _haut, float proba, float coef);
	void set(const Fwelcome* fwel);
	void reset(int _larg, int _haut, float proba, float coef);

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
