#include "qterminal.h"
#include <QtGui/qevent.h>
using namespace std;
// Constructeur
Qterminal::Qterminal(int _hauteur, int _largeur, float _proba, long int _temps){
	this->temps = _temps;
		this->foret = new Foret(_hauteur, _largeur, _proba);
		this->buffer= new QImage();
		this->color = new QColor(Qt::black);
}


void Qterminal::dessine_grille(QPainter& paint)
{
	int _width = width() / this->foret->hauteur();
    int _height = height() / this->foret->hauteur();
	
	for(int i = 0; i<= this->foret->largeur() ; ++i){
		vector< Cellule* >* ligne= foret->operator[](i);
		for( int j = 0; j <= this->foret->hauteur() ; ++j){
				
			// Selon l'état de la cellule où nous nous trouvons, on met à jour la couleur nécessaire
			if((*ligne)[j]->getEtat() == 0){
				this->color->setNamedColor("black");
				
			}else if((*ligne)[j]->getEtat() == 1){
				// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
				unsigned int indice= dynamic_cast < const Arbre* >((*ligne)[j])->getEssence()->getIndice();
				switch(indice){
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
				
			}else if((*ligne)[j]->getEtat() == 2){
				this->color->setNamedColor("red");	
				
			}else if((*ligne)[j]->getEtat() ==3){
				this->color->setNamedColor("gray");
			}
			
			paint.fillRect(i*_width,j*_height,_width,_height,*(this->color));
			
		}
	}	
}


void Qterminal::paintEvent(QPaintEvent* event)
{
	QPainter paint(this);
    paint.drawImage(QPoint(0,0),*(this->buffer));
}

void Qterminal::resizeEvent(QResizeEvent* event)
{
	if(this->buffer->isNull()){
      // Redimensionnement manuel
      this->setMinimumSize(600,600);
      this->buffer = new QImage(600,600,QImage::Format_ARGB32); 
      this->buffer->fill(1);
    }
    else{
      // Vérifier le redimensionnement de la QImage
      this->buffer = new QImage(event->size().width(),event->size().height(),QImage::Format_ARGB32);
      this->buffer->fill(1);
    }
    
    QPainter paint(this->buffer);
	dessine_grille(paint);
}

Qterminal::~Qterminal()
{

}
