#ifndef WINDWIDGET_H
#define WINDWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>

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
	QLabel* vitesse_lbl;

	int vitesse;

public:
	/* Constructeur et destructeur */
	WindWidget();
	virtual ~WindWidget();
	
	/* Méthodes d'initialisation */
	void initComponents();
	
	/* Getters */
	int get_angle() const { return wind->getAngle(); };
	int get_vitesse() const { return vitesse; };
	
protected:
	void resizeEvent(QResizeEvent* Qevent);

public slots:
	/* Setters */
	void set_angle(int x);
	void set_vitesse(int y);
	
signals:
	void modif_value(int);
};

#endif // WINDWIDGET_H
