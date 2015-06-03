#ifndef WINDWIDGET_H
#define WINDWIDGET_H

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QCheckBox>

#include <iostream>

#include "../debug.h"
#include "windcircle.h"

/**
 * Classe représentant le widget complet pour gérer orientation et
 * vitesse du vent de manière dynamique
 * @author Ugo
 * 
 */
class WindWidget : public QWidget
{
Q_OBJECT
private:
	WindCircle* wind;
	QLabel* angle_lbl;
	QLabel* speed_lbl;
	QTimer* timer;
	QCheckBox* varAngleBox;
	QSlider* slider_vitesse;

// 	bool varWind;
	int speed;

public:
	/* Constructeur et destructeur */
	WindWidget();
	virtual ~WindWidget();
	
	/* Méthodes d'initialisation */
	void initComponents();
	void initValues(int angle, int vitesse);
	
	/* Getters */
	int getAngle() const { return wind->getAngle(); };
	int getSpeed() const { return speed; };
	
	/* Setters */
private:
	void setAngle(int angle);
	
protected:
	void resizeEvent(QResizeEvent* Qevent);

public slots:
	void majAngle();
	void majSpeed(int vitesse);
	void changeWindDir();
	
signals:
	void modif_value(int, int);
};

#endif // WINDWIDGET_H
