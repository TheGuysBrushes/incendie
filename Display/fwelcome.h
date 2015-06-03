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
#include <QtGui/QFileDialog>
// Pour les valeurs max des SpinBox
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>

#include <iostream>
#include <fstream>
#include <ctime>

#include "../debug.h"

enum DialogCode{Rejected, Accepted, Load, Restore, RestoreSeed};

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
	
	QFileDialog* fileDialog;
	
	float proba;
	float burningCoef;
	std::time_t seed;
	
	std::ifstream* file;

public:
	/* Constructeur et destructeur */
	Fwelcome(QWidget* parent);
	Fwelcome(QWidget* parent, int _largeur, int _hauteur);
	virtual ~Fwelcome();

	/* Autres Méthodes */
	void createComponents();
	void initComponents();
	void addCancel() const;

	/* Getters */
	int getHaut() const	{ return haut_spin->value(); };
	int getLarg() const	{ return larg_spin->value(); };
	float getProba() const	{ return proba; };
	float getCoef() const	{ return burningCoef; };
	float getSeed() const	{ return seed; };
	
	std::ifstream* getFile() { return file; };
	QImage* getImage() { return pictureForest; };
// 	QPushButton* getRestoreBtn() { return restoreBtn; };

public slots:
	/* Setters */
	// Slots d'ouverture FileDialog pour chargement
	void popImageDIalog();
	void popSaveDialog();
// 	void popSeedDialog();
	
	void set_proba(int x);
	void set_coef(int x);
	void restore(QString filename);
	void loadFromImg(QString filename);
};

#endif // FWELCOME_H
