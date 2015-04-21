#include "firewidget.h"
#include <QtGui/qevent.h>

using namespace std;

// Constructeur
FireWidget::FireWidget(int _hauteur, int _largeur, float _proba, long int _temps)
	: foret(_hauteur,_largeur,_proba), temps(_temps), running(false)
{
	buffer = new QImage();
	color = new QColor(Qt::black);
	
	setMinimumSize(600,600);
}

// Destructeur
FireWidget::~FireWidget()
{
	delete(buffer);
	delete(color);
}

//Autres méthodes
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
	QPainter paint(this->buffer);
	
	int current_largeur= 0;
	for(int i=0; i<this->foret.largeur(); ++i){
		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
		// éviter la petite zone en bas de grille
		vector< Cellule* >* ligne= foret[i];
		
		int current_hauteur= 0;
		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
			Cellule* cell= *j;
			
			if( cell->getEtat() == 0){
				this->color->setNamedColor("black");
				paint.fillRect(current_largeur, current_hauteur, cell_larg, cell_haut, *(color));
			}
			else if(cell->getEtat() == 1){
				// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
				unsigned indice= dynamic_cast < const Arbre* >(cell)->getEssence()->getIndice();
				setColor(indice);
				paint.fillRect(current_largeur, current_hauteur, cell_larg, cell_haut, *(color));
			}
			
			// Incrémentations des positions des cellules
			current_hauteur += cell_haut;
		}
		current_largeur += cell_larg;
	}
}

/**
 * Dessine la matrice dans le buffer, sur l'ancienne matrice
 * 	On réutilise les cellules non susceptibles d'avoir été modifiées
 * @author Ugo and Florian
 */
void FireWidget::drawFire()
{
	QPainter paint(this->buffer);
	
	int current_largeur= 0;
	for(int i=0; i<this->foret.largeur(); ++i){
		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
		// éviter la petite zone en bas de grille
		vector< Cellule* >* ligne= foret[i];
		
		int current_hauteur= 0;
		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
			Cellule* cell= *j;
			
			if(cell->getEtat() == 2){
				this->color->setNamedColor("red");
				paint.fillRect(current_largeur, current_hauteur, cell_larg, cell_haut, *(color));
				
			}else if(cell->getEtat() ==3){
				this->color->setNamedColor("gray");
				paint.fillRect(current_largeur, current_hauteur, cell_larg, cell_haut, *(color));
			}			
			
			// 			paint.fillRect(current_largeur, current_hauteur, cell_larg, cell_haut, *(color));
			
			// Incrémentations des positions des cellules
			current_hauteur += cell_haut;
		}
		current_largeur += cell_larg;
	}
}

/**
 * IMPROVEIT
 * J'ai essayé de créer une fonction qui dessinerai les cellules grâce à l'appel d'une autre fonction donné en paramètre,
 * 	mais "function/std::function/function::function avec #include <functional> ne marche pas
 * @deprecated
 * @author Florian
 */

/**
 * premiere fonction, il aurait fallu créer drawStatic ...
 */
// void FireWidget::drawVariable(int posWidth, int posHeight, const Cellule* cell)
// {
// 	int cell_larg = width() / this->foret.largeur();
// 	int cell_haut = height() / this->foret.hauteur();
// 	
// 	QPainter paint(this->buffer);
// 	
// 	if( cell->getEtat() == 0){
// 		this->color->setNamedColor("black");
// 		
// 	}else if(cell->getEtat() == 1){
// 		// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
// 		unsigned indice= dynamic_cast < const Arbre* >(cell)->getEssence()->getIndice();
// 		setColor(indice);
// 	}
// 	paint.fillRect(posWidth, posHeight, cell_larg, cell_haut, *(color));	
// }


// void FireWidget::drawForest(void(*pDraw)(int, int, const Cellule*) )
// {
// 	int cell_larg = width() / this->foret.largeur();
// 	int cell_haut = height() / this->foret.hauteur();
// 	
// 	int current_largeur= 0;
// 	for(int i=0; i<this->foret.largeur(); ++i){
// 		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
// 		// éviter la petite zone en bas de grille
// 		vector< Cellule* >* ligne= foret[i];
// 		
// 		int current_hauteur= 0;
// 		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
// 			Cellule* cell= *j;
// 
// 			pDraw(cell_larg,cell_haut, cell);
// 			
// 	// Incrémentations des positions des cellules
// 			current_hauteur += cell_haut;
// 		}
// 		current_largeur += cell_larg;
//     }
// }

// #############
// 	Events
// #############
void FireWidget::paintEvent(QPaintEvent* event)
{
	QPainter paint(this);
	paint.drawImage(0, 0, *buffer);
}

void FireWidget::resizeEvent(QResizeEvent* event)
{	
	if (!buffer->isNull()){
		delete(buffer);
	}
	
	int tailleCote= min(event->size().width(), event->size().height());
	#if DEBUG_TMATRICE
	cout << "tH: "<< event->size().width()<< " tL "<< event->size().height()<< " ; min donne taille coté : " << tailleCote<< endl;
	#endif
	
// 	resize(tailleCote,tailleCote);
	
	cell_larg = event->size().width() /	(float)this->foret.largeur();
	cell_haut = event->size().height() /	(float)this->foret.hauteur();
	
	buffer = new QImage(tailleCote,tailleCote, QImage::Format_ARGB32);
	buffer->fill(Qt::white);
// 	void(*pDraw)(int, int, const Cellule*);
// 	pDraw= drawVariable;
	drawForest();
	drawFire();
// 	update(); // deja fais apres l'appel à resizeEvent ?
}

void FireWidget::next()
{
	// pas suivant
	foret.NextMove();
	
	// mise à jour de l'image puis affichage à l'écran
	drawFire();
	update();
}

void FireWidget::run()
{
	running= true;
	// tant que la foret est modifiee
	while (foret.NextMove() && running) {
		drawFire();
		update();
		usleep(temps);
	}
}

void FireWidget::pause()
{
	running= false;
}

