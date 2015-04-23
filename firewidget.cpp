#include "firewidget.h"
#include <QtGui/qevent.h>

using namespace std;

// Constructeur
FireWidget::FireWidget(int hauteur, int largeur, float proba, float coef_brulure)
{
	foret = new Foret(hauteur,largeur,proba,coef_brulure);
	buffer = new QImage();
	color = new QColor(Qt::black);
	
	setMinimumSize(largeur, hauteur);
	
	probaMatriceReset= proba;
}

// Destructeur
FireWidget::~FireWidget()
{
	delete(buffer);
	delete(color);
	delete(foret);
}

// #####################
//		Autres méthodes
// #####################
void FireWidget::setColor(int colorIndice)
{
	switch(colorIndice){
		case 0:
			this->color->setRgb(1,100,0,255);
			break;
		case 1:
			this->color->setRgb(34,139,34,255);
			break;
		case 2:
			this->color->setRgb(107,142,35,255);
			break;
		case 3:
			this->color->setRgb(0,205,0,255);
			break;
		case 4:
			this->color->setRgb(46,139,87,255);
			break;								
	}
}

// #################
//		Affichages
// #################

/**
 * Dessine les arbres et cellules vides dans le buffer
 * @author Florian and Ugo
 */
void FireWidget::drawForest()
{	
	QPainter paint(buffer);
	
	int current_hauteur= 0;
	for(int i=0; i<foret->hauteur(); ++i){
		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
		// éviter la petite zone en bas de grille
		vector< Cellule* >* ligne= (*foret)[i];
		
		int current_largeur= 0;
		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
			Cellule* cell= *j;
			
			if( cell->getEtat() == 0){
				this->color->setNamedColor("black");
				paint.fillRect(current_largeur, current_hauteur, tailleCell, tailleCell, *(color));
			}
			else if(cell->getEtat() == 1){
				// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
				Arbre* ab= dynamic_cast < Arbre* >(cell);
				unsigned indice= ab->getEssence()->getIndice();
//				Pour changer la couleur de l'arbre en train d'etre enflammé 
// 				int brulure= ab->getEssence()->get;
				setColor(indice );
				#if DEBUG_TMATRICE
				cout <<"draw cell ; ";
				#endif
				paint.fillRect(current_largeur, current_hauteur, tailleCell, tailleCell, *(color));
			}
			
			// Incrémentations des positions des cellules
			current_largeur += tailleCell;
		}
		#if DEBUG_TMATRICE
		cout << endl;
		#endif
		current_hauteur += tailleCell;
	}
	
	#if DEBUG_TMATRICE
	cout <<"fin draw forest ; "<< endl;
	#endif
}

/**
 * Dessine la matrice dans le buffer, sur l'ancienne matrice
 * 	On réutilise les cellules non susceptibles d'avoir été modifiées
 * @author Florian and Ugo
 */
void FireWidget::drawFire()
{
	QPainter paint(buffer);
	
	list< Arbre * >* onFire= &(foret->onFire);
	for( list< Arbre * >::const_iterator j( onFire->begin() ); j != onFire->end(); ++j){
		color->setNamedColor("red");
		paint.fillRect(tailleCell* (*j)->getPos().col, tailleCell* (*j)->getPos().row, tailleCell, tailleCell, *(color));
	}
	
	const list< Arbre * >* newAsh= foret->getCarbonized();
	for( list< Arbre * >::const_iterator j( newAsh->begin() ); j != newAsh->end(); ++j){
		color->setNamedColor("gray");
		paint.fillRect(tailleCell* (*j)->getPos().col, tailleCell* (*j)->getPos().row, tailleCell, tailleCell, *(color));
	}
	foret->clearCarbonized();
}


bool FireWidget::eteindreFeu(int ligne, int colonne)
{
	#if DEBUG_ALLUME
	cout << "eteindre cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
	#endif
	
	if(ligne>=0 && ligne < foret->hauteur()){
		vector< Cellule* >* line= foret->operator[](ligne);
		
		if (colonne>=0 && colonne < foret->largeur()){
			Cellule* cell= (*line)[colonne];	
			
			if (cell->getEtat()==2){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				foret->water(ab);
				drawFire();
				update();
				
				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}

bool FireWidget::allumerFeu(int ligne, int colonne)
{
#if DEBUG_ALLUME
	cout << "allumer cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
#endif
	
	if(ligne>=0 && ligne < foret->hauteur()){
		vector< Cellule* >* line= foret->operator[](ligne);
		
		if (colonne>=0 && colonne < foret->largeur()){
			Cellule* cell= (*line)[colonne];	
		
			if (cell->getEtat()==1){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				foret->allumer(ab);
				drawFire();
				update();
				
				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}

bool FireWidget::finirFeu(int ligne, int colonne)
{
	#if DEBUG_ALLUME
	cout << "embraser cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
	#endif
	
	if(ligne>=0 && ligne < foret->hauteur()){
		vector< Cellule* >* line= foret->operator[](ligne);
		
		if (colonne>=0 && colonne < foret->largeur()){
			Cellule* cell= (*line)[colonne];	
			
			if (cell->getEtat()==2){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				foret->eteindre(ab);
				drawFire();
				update();
				
				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}


// #############
// 	Events
// #############
void FireWidget::paintEvent(QPaintEvent* event)
{
	QPainter paint(this);
	paint.drawImage(0, 0, *buffer); // , 0, 0, width(), height());
}

void FireWidget::resizeEvent(QResizeEvent* event)
{	
	if (!buffer->isNull()){
		delete(buffer);
	}
	
	float nbCol= foret->largeur();
	float nbRow= foret->hauteur();
	tailleCell = min (event->size().width() / nbCol , event->size().height() / nbRow);
	
	#if DEBUG_DIMENSION
	cout << "tH: "<< event->size().width()<< " tL "<< event->size().height()<< endl;
	cout << "taille Cellule : "<< tailleCell<< endl;
	#endif
	
// 	/*resize*/setMinimumSize(tailleCell*foret->largeur(), tailleCell*foret->hauteur());

	buffer = new QImage(tailleCell*foret->largeur(), tailleCell*foret->hauteur(), QImage::Format_ARGB32);
	buffer->fill(Qt::white);
	drawForest();
	drawFire();
}

void FireWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event); // TODO verifier si à supprimmer
	
	int colonne= event->x()/tailleCell;
	int ligne= event->y()/tailleCell;
	
	if (event->button()==Qt::LeftButton)
		allumerFeu(ligne, colonne);
	else if (event->button()==Qt::MiddleButton)
		finirFeu(ligne, colonne);
	else if (event->button()==Qt::RightButton){
		eteindreFeu(ligne, colonne);
		drawForest();
	}
}


void FireWidget::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);	// TODO verifier si à supprimmer
	
	int colonne= event->x()/tailleCell;
	int ligne= event->y()/tailleCell;
	
	if (event->buttons().testFlag(Qt::LeftButton) )
		allumerFeu(ligne, colonne);
	else if (event->buttons().testFlag(Qt::MiddleButton) )
		finirFeu(ligne, colonne);
	else if (event->buttons().testFlag(Qt::RightButton) ){
		eteindreFeu(ligne, colonne);
		drawForest();
	}
}

void FireWidget::newForet(int _hauteur, int _largeur, float _proba, float _coef_brulure)
{
	delete(foret);
	foret = new Foret(_hauteur,_largeur,_proba,_coef_brulure);
}

// ################
// 	Deroulement
// ################
/**
 * Passe de l'etat t à t+1 la foret
 * @author Florian
 * @return vrai si la foret a ete modifiée
 */
bool FireWidget::next()
{
	// pas suivant
	if (!foret->NextMove())
		return false;
	
	// mise à jour de l'image puis affichage à l'écran
	drawFire();
	update();
	
	return true; // cas par défaut, il y a eu un changement
}


/**
 * reinitialise la foret
 * @author Florian et un petit peu Ugo :p
 */
void FireWidget::restart()
{
	Foret* OldForet= foret;
	foret = new Foret(*OldForet, probaMatriceReset);
	delete(OldForet);
	// il faudrait en plus vider la matrice de feu ...
// 	foret->reset(probaMatriceReset);
	drawForest();
	drawFire();
	update();
}

