#ifndef FWELCOME_H
#define FWELCOME_H

#include "../debug.h"

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
#include <QtGui/QProgressBar>
/**
 * Classe représentant le panneau de dialogue
 * pour la sélection des paramètres de l'automate
 * @author Ugo
 */
class Fwelcome : public QDialog
{
Q_OBJECT
private:
	QSpinBox* haut_spin;
	QSpinBox* larg_spin;
	QLabel* p_value;
	QLabel* c_value;
// 	QPushButton* valid_btn;
	QPushButton* cancel_btn;
	QProgressBar* PB_load;
	
	float proba;
	float coef_brulure;
	std::string directory; 

public:
	/* Constructeur et destructeur */
	Fwelcome(QWidget* parent);
	virtual ~Fwelcome();

	/* Autres Méthodes */
	void initComponents();
	void addCancel() const;

	/* Getters */
	int get_haut() const	{ return haut_spin->value(); };
	int get_larg() const	{ return larg_spin->value(); };
	float get_proba() const	{ return proba; };
	float get_coef() const	{ return coef_brulure; };
	std::string getDirectory() const { return directory; };
// 	QProgressBar* getProgressBar() const	{ return PB_load; };

public slots:
	/* Setters */
	void set_proba(int x);
	void set_coef(int x);
	void load(/*std::string filename*/);
};

#endif // FWELCOME_H
