#include "windcircle.h"
#include <math.h>
#include <QtCore/QString>

using namespace std;
#define PI 3.14159265

/*** Constructeur et destructeur ***/
WindCircle::WindCircle(int x):angle(x){
	buffer = new QImage();
	center = new QPointF((float)(width()/2.0),(float)(height()/2.0));
	direction = new QPointF();
	setMinimumSize(100,100);
}

WindCircle::~WindCircle(){

}
/*** Getters and Setters ***/
void WindCircle::setAngle(int x)
{
	angle = x;
}


/*** Autres Méthodes ***/
void WindCircle::drawCircle(){
	QPainter paint(this->buffer);
	float rayon = height()/2.0 - 25.0;
	paint.drawEllipse(*center,rayon, rayon);
	
	// Dessin des 4 lignes cardinales
	paint.drawLine(*center, QPointF(center->rx(), center->ry()+rayon+6)); // Sud
	paint.drawLine(*center, QPointF(center->rx(), center->ry()-rayon-6)); // Nord
	paint.drawLine(*center, QPointF(center->rx()+rayon+6, center->ry())); // Est
	paint.drawLine(*center, QPointF(center->rx()-rayon-6, center->ry())); // Ouest
	
	// Dessin des caractères cardinaux (NSEO)
	paint.drawText(QPointF(center->rx()-4, center->ry()+rayon+20), QString("S"));
	paint.drawText(QPointF(center->rx()-4, center->ry()-rayon-10), QString("N"));
	paint.drawText(QPointF(center->rx()+rayon+10, center->ry()+4), QString("E"));
	paint.drawText(QPointF(center->rx()-rayon-20, center->ry()+4), QString("O"));
	
}

void WindCircle::drawDir(){
	effaceBuffer();
	setDirection(angle);
	QPainter paint(this->buffer);
	QPen pen(Qt::red);paint.setPen(pen);

	paint.drawLine(*center, *direction);
	
	
}

void WindCircle::effaceBuffer()
{
	buffer->fill(1);
	drawCircle();
}

void WindCircle::setDirection(int angle){
	float rayon =(float)height()/2.0 - 25.0;
	direction->setX(cos(PI*(float)angle/180.0)*rayon+center->rx());
	direction->setY(sin(PI*(float)angle/180.0)*rayon+center->ry());
}



/*** Events ***/
void WindCircle::paintEvent(QPaintEvent* event){
	QPainter paint(this);
	paint.drawImage(0, 0, *buffer);
}

void WindCircle::resizeEvent(QResizeEvent* event){
    if(this->buffer->isNull()){
      // Redimensionnement manuel
      this->setMinimumSize(100,100);
      this->buffer = new QImage(100,100,QImage::Format_ARGB32); 
      this->buffer->fill(1);
    }
    else{
      // Vérifier le redimensionnement de la QImage
      this->buffer = new QImage(event->size().width(),event->size().height(),QImage::Format_ARGB32);
      this->buffer->fill(1);

    }

	center->setX(event->size().width()/2);
	center->setY(event->size().height()/2);
	setDirection(angle);
  	drawCircle();
	drawDir();
}

/*** Slots ***/