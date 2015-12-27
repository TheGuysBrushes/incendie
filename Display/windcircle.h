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
    /**
     * Constructeur de classe
     * @param int Valeur de l'angle initial
     * @author Ugo
     */
	WindCircle();
	virtual ~WindCircle();

/* Getters et Setters */
    int getAngle() const { return angle; }

    /**
     * Met à jour la valeur de l'angle et redessine la ligne d'angle
     * @param int Valeur de l'angle
     * @author Ugo
     */
	void setAngle(int x);
    /**
     * Permet de calculer le point d'arrivée du segment
     * sur le cercle trigonométrique en fonction de l'angle passé
     * en paramètre.
     * @param int Valeur de l'angle choisi
     * @author Ugo
     */
	void setDirection(int angle);

/* Méthodes graphiques */
    /**
     * Affiche le cercle de la boussole
     * @author Ugo
     */
    void drawCircle();
    /**
     * Affiche la ligne directionnelle de la boussole
     * @author Ugo
     */
    void drawDir();
    /**
     * Vide le buffer
     * @author Ugo
     */
    void cleanBuffer();

protected:
/* Events */
    /**
     * Copie le buffer sur le widget
     * @author Ugo
     */
	void resizeEvent(QResizeEvent * event);
    /**
     * Replace le centre du cercle et redessine la ligne de l'angle
     * @author Ugo
     */
    void paintEvent(QPaintEvent* event);
    /**
     * Nous avons redéfini cet Event dans le but de pouvoir faire varier la valeur de l'angle
     * dynamiquement à l'aide du clic sur le widget
     * Signale à windwidget que l'angle est modifié
     * @author Ugo
     */
	void mousePressEvent(QMouseEvent* event);
signals:
	void modifAngle();
};

#endif // WINDCIRCLE_H
