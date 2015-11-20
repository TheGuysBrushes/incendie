#ifndef WINDCIRCLE_H
#define WINDCIRCLE_H

#include <QtCore/QString>
#include <QtCore/QPointF>
#include <QtCore/QEvent>
#include <QtGui/qevent.h>	// event de la partie graphique
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>
#include <QtGui/QImage>

#include <cmath>
#include <iostream>

/**
 * Cette classe correspond au widget d'affichage de la direction
 * du vent. Elle fonctionne comme une boussole et utilise un
 * double buffering pour l'affichage
 * @author Ugo
 * 
 */
class WindCircle : public QWidget
{
Q_OBJECT
private:
	QImage* buffer;
	QPointF* center;
	QPointF* direction;
	int angle;

public:
	/* Constructeur et destructeur */
	WindCircle();
	virtual ~WindCircle();
	
	/* Méthodes graphiques */
	void drawCircle();
	void drawDir();
	void effaceBuffer();

	/* Getters et Setters */
	int getAngle() const { return angle; };
	
	void setAngle(int x);
	void setDirection(int angle);

protected:
	/* Events */
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
signals:
	void modifAngle();
};

#endif // WINDCIRCLE_H
