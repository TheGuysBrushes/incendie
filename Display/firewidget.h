#ifndef FIREWIDGET_H
#define FIREWIDGET_H

#include <QtCore/QByteArray>
#include <QtCore/QPoint>

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QRubberBand>
#include <QtGui/qevent.h>	// IMPROVEIT pas d'equivalent sans .h ?

#include <vector>
#include <fstream>

// Macros
#include "../Moteur/actions.h"
#include "../debug.h"

#include "../Moteur/foret.h"
#include "fwelcome.h"

/**
 * Widget d'affichage de l'automate cellulaire. Ce composant
 * permet de suivre graphiquement l'évolution de l'automate. 
 * Il utilise un système de buffering et s'appuie sur la classe
 * Foret du moteur.
 * @author Ugo et Florian
 */
class FireWidget : public QWidget {
Q_OBJECT
private:
	Foret* forest;
	QImage* buffer;
	QColor* color;
	QPainter* bufferPainter;

// 	QImage* pictureForest;
	
	QPoint origin;
	QRubberBand* rubber;
	// Points de départ et d'arrivée de la zone de selection (redondance pour origine mais normal pour l'instant)
	QPoint depart;
	QPoint arrivee;
	
	qreal tailleCell;
	long temps;
	bool running;
	
private:
	void initRubber(QMouseEvent* event);
	
public:
	/* Constructeur et desctructeur */
	FireWidget();
	virtual ~FireWidget();

	/* Initialisations */
	void initialise(int largeur, int hauteur, float proba = 0.60, float coef_brulure=1.0);
	void initialise(int largeur, int hauteur, std::ifstream* file);
	bool initialise(QImage* imageForet);

	/* Getters et Setters */
	void setColor(int colorIndice);	
	void setWind(float angle, float vitesse)	{ forest->setWind(angle, vitesse); };
	int getTailleCell() const { return tailleCell; };
	Foret* getForet() const { return forest; };
	void razRubber() 	{ rubber= NULL; };
	
	/* Gestion Foret */
	void loadFromPicture(int largeurImage, int hauteurImage, QImage* imageForet);
	bool loadForest(std::string filename);
	void saveForest() const;
	void delForest();

	bool eteindreFeu(int colonne, int ligne);
	bool allumerFeu(int colonne, int ligne);
	bool finirFeu(int colonne, int ligne);
// 	void newForet(int _largeur, int _hauteur, float _proba, float _coef_brulure);

	
	/* Affichage */
	void drawPicture();
	void drawCell(int colonne, int ligne);
	void drawTree(const Arbre* ab);
	void drawList(std::list< Arbre* >* arbres);
	void drawForest();
	void drawChanged();
	void redraw();

	/** @deprecated */
// 	void reset(int _larg, int _haut, float proba, float coef);

protected:
	/* Events */
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
public slots:
	bool next();
	void actionReceived(int x);
	
signals:
	void releaseSignal();
	
};

#endif // FIREWIDGET_H
