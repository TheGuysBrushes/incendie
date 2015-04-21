#ifndef FIREWIDGET_H
#define FIREWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <vector>

#include "foret.h"

class FireWidget : public QWidget {
private:
	QImage* buffer;
	QColor* color;
	Foret foret;
	long temps;
	
public:
	// Constructeur et desctructeur
	FireWidget(int _hauteur, int _largeur, float _proba = 0.60, long _temps=150000);
	virtual ~FireWidget();
	
	// Autres méthodes
	void fill_buffer(QPainter &paint);

protected:
// 	void mousePressEvent(QMouseEvent *event);
// 	void mouseMoveEvent(QMouseEvent *event);
// 	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
};

#endif // FIREWIDGET_H
