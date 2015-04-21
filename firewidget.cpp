#include "firewidget.h"
#include <QtGui/qevent.h>

using namespace std;

// Constructeur
FireWidget::FireWidget(int _hauteur, int _largeur, float _proba, long int _temps)
:foret(_hauteur,_largeur,_proba),temps(_temps){
	setAttribute(Qt::WA_PaintOutsidePaintEvent);
	buffer = new QImage();
	color = new QColor(Qt::black);
	
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



// Dessine la matrice dans le buffer
void FireWidget::fill_buffer(QPainter& paint)
{
	int cell_larg = width() / this->foret.largeur();
	int cell_haut = height() / this->foret.hauteur();
	
	int current_largeur= 0;
	for(int i=0; i<this->foret.largeur(); ++i){
		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
		// éviter la petite zone en bas de grille
		vector< Cellule* >* ligne= foret.operator[](i);
		
		int current_hauteur= 0;
		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
			Cellule* cell= *j;
			
			if( cell->getEtat() == 0){
				this->color->setNamedColor("black");
				
			}else if(cell->getEtat() == 1){
				// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
				unsigned indice= dynamic_cast < const Arbre* >(cell)->getEssence()->getIndice();
				setColor(indice);
				
			}else if(cell->getEtat() == 2){
				this->color->setNamedColor("red");	
				
			}else if(cell->getEtat() ==3){
				this->color->setNamedColor("gray");
			}			
			
			paint.fillRect(current_largeur, current_hauteur, cell_larg, cell_haut, *(color));
			
			current_hauteur += cell_haut;
		}
		current_largeur += cell_larg;
    }
}


// Events
void FireWidget::paintEvent(QPaintEvent* event)
{
	QPainter paint(this);
	paint.drawImage(0, 0, *buffer);
}

void FireWidget::resizeEvent(QResizeEvent* event)
{
	if(this->buffer->isNull()){
      // Redimensionnement manuel
      setMinimumSize(600,600);
      buffer = new QImage(600,600,QImage::Format_ARGB32); 
      buffer->fill(1);
    }
    else{
      // Vérifier le redimensionnement de la QImage
      buffer = new QImage(event->size().width(),event->size().height(),QImage::Format_ARGB32);
      buffer->fill(1);
    }
    
    QPainter paint(this->buffer);
	fill_buffer(paint);
}

void FireWidget::next()
{
	foret.NextMove();
	buffer->fill(1);
	
	QPainter peintre(this);
	
	fill_buffer(peintre);
// 	update();
}

