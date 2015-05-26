#ifndef FWELCOME_H
#define FWELCOME_H

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
#include <QtGui/QProgressBar>
#include <QtGui/QGridLayout>
#include <QtGui/QImage>
// Pour les valeurs max des SpinBox
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>

#include <fstream>

#include "../debug.h"

/**
 * Classe représentant le panneau de dialogue
 * pour la sélection des paramètres de l'automate
 * @author Ugo
 */
class Fwelcome : public QDialog
{
Q_OBJECT
private:
	QGridLayout* gridLay;
	QSpinBox* haut_spin;
	QSpinBox* larg_spin;
	
	QLabel* p_value;
	QLabel* c_value;
	
	QPushButton* valid_btn;
	QPushButton* cancel_btn;
	QPushButton* loadFromImgBtn;
	QPushButton* restoreBtn;
	
	QImage* pictureForest;
	
	float proba;
	float coef_brulure;
	std::ifstream* file;

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
	std::ifstream* getFile() { return file; };
	QImage* getImage() { return pictureForest; };
// 	QPushButton* getRestoreBtn() { return restoreBtn; };

public slots:
	/* Setters */
	void set_proba(int x);
	void set_coef(int x);
	void restore(/*std::string filename*/);
	void loadFromImg();
};

#endif // FWELCOME_H
