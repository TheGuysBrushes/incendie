#ifndef FIREWIDGET_H
#define FIREWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>

#include <vector>

#include <QtCore/QByteArray>
#include <QtCore/QPoint>
#include <QtGui/QRubberBand>
#include "../Moteur/foret.h"
#include "fwelcome.h"

// TODO Modifier le nom de la fonction drawFire pour exprimer le dessin des cellules qui ont changé d'état +
// 	TODO voir si on fait une liste unique pour les éléments qui ont changé d'état
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
	int tailleCell;

	QPixmap* pictureForest;
	
	QPoint origin;
	QPoint arrivee;
	QRubberBand* rubber;
	long temps;
	bool running;

public:
	/* Constructeur et desctructeur */
	FireWidget(int _largeur, int _hauteur, float proba = 0.60, float coef_brulure=1.0);
	FireWidget();
	virtual ~FireWidget();

	/* Autres Méthodes */
	void initialise(int _largeur, int _hauteur, float proba = 0.60, float coef_brulure=1.0);
	void delForet();

	bool loadPicture(QString filename);
	bool eteindreFeu(int colonne, int ligne);
	bool allumerFeu(int colonne, int ligne);
	bool finirFeu(int colonne, int ligne);
// 	void newForet(int _largeur, int _hauteur, float _proba, float _coef_brulure);

	/* Getters et Setters */
	void setColor(int colorIndice);	
	void setWind(float _hor, float _ver);
	
	/* Affichage */
	void drawPicture();
	void drawCell(int colonne, int ligne);
	void drawTree(const Arbre* ab);
	void drawList(std::list< Arbre* >* arbres);
	void drawForest();
	void drawChanged();
	void redraw();

	/** @deprecated */
	void reset(int _larg, int _haut, float proba, float coef);

protected:
	/* Events */
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
public slots:
	bool next();
// 	void drawFire();
// 	void transition(long x);
	void actionReceived(int x);
	
signals:
	void releaseSignal();
	
};

#endif // FIREWIDGET_H
