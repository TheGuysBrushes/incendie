#ifndef FWELCOME_H
#define FWELCOME_H
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
/**
 * Classe représentant le panneau de dialogue
 * pour la sélection des paramètres de l'automate
 * @author Ugo
 */
class Fwelcome : public QDialog
{
Q_OBJECT
private:
	QPushButton* valid_btn;
	QSpinBox* haut_spin;
	QSpinBox* larg_spin;
	QLabel* p_value;
	QLabel* c_value;
	float proba;
	float coef_brulure;
	QPushButton* cancel_btn;
	
public:
	/* Constructeur et destructeur */
	Fwelcome(QWidget* parent);
	virtual ~Fwelcome();
	
	/* Autres Méthodes */
	void initComponents();
	
	/* Getters */
	int get_haut() const{ return haut_spin->value(); };
	int get_larg() const{ return larg_spin->value(); };
	float get_proba() const{ return proba; };
	float get_coef() const{ return coef_brulure; };
	QPushButton* get_cancel() const { return cancel_btn; };
	
public slots:
	/* Setters */
	void set_proba(int x);
	void set_coef(int x);
};

#endif // FWELCOME_H
