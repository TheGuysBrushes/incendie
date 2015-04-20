#ifndef QTERMINAL_H
#define QTERMINAL_H

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtCore/QPoint>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <vector>

#include "foret.h"

class Qterminal: public QWidget
{
private:
	QImage* buffer;
	QColor* color;
	Foret* foret;
	long temps;

public:
    Qterminal(int _hauteur, int _largeur, float _proba = 0.60, long _temps=150000);
	virtual ~Qterminal();
	
//     void efface_buffer();
    void dessine_grille(QPainter &paint);
    
protected:
// 	void mousePressEvent(QMouseEvent *event);
//   void mouseMoveEvent(QMouseEvent *event);
//   void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
  
};

#endif // QTERMINAL_H
