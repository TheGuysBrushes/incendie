#include "windcircle.h"

#include "../debug.h"
// Valeur du nombre pi, utilisée pour les calcul de trigonométrie
#define PI 3.14159265
// doit etre positif strictement, sinon il n'y a pas de variation
#define WIND_VARIATION 9

using namespace std;

/*** Constructeur et destructeur ***/
WindCircle::WindCircle()
{
	buffer = new QImage();
	center = NULL;
	direction = new QPointF();

	setMinimumSize(100,100);
	setMaximumSize(180,180);
	
	this->heightForWidth(1);
}

WindCircle::~WindCircle(){
	delete(buffer);
	delete(center);
	delete(direction);
}


/*** Getters and Setters ***/
void WindCircle::varyAngle(){
    // Choisi une valeur entre -WIND_VARIATION et WIND_VARIATION
    float coef = rand()%(2*WIND_VARIATION +1) - WIND_VARIATION;
#if DEBUG_VAR
    cout << "coef variation : "<< coef<< endl;
#endif
    // change modifie la valeur de l'angle avec la valeur choisie
    int new_angle = angle + coef;
    if(new_angle > 630)
        new_angle -= 360;
    if(new_angle < 270)
        new_angle += 360;
    #if DEBUG_VAR
    cout << "angle après modif " << angle << endl;
    #endif
    updateAngle(new_angle);
}

void WindCircle::updateAngle(int x){
    angle = x;
    drawDir();
    update();
    emit modifAngle();
}

void WindCircle::setDirection(int angle){
	float rayon =(float)height()/2.0 - 25.0;
	direction->setX(cos(PI*(float)angle/180.0)*rayon+center->rx());
	direction->setY(sin(PI*(float)angle/180.0)*rayon+center->ry());
}

/*** Méthodes Graphiques ***/
void WindCircle::drawCircle(){
	QPainter paint(buffer);
	
	float rayon = height()/2.0 - 25.0;
	
	paint.drawEllipse(*center, rayon, rayon);
	
	// Dessin des 4 lignes cardinales
	paint.drawLine(*center, QPointF(center->rx(), center->ry()+rayon+6)); // Sud
	paint.drawLine(*center, QPointF(center->rx(), center->ry()-rayon-6)); // Nord
	paint.drawLine(*center, QPointF(center->rx()+rayon+6, center->ry())); // Est
	paint.drawLine(*center, QPointF(center->rx()-rayon-6, center->ry())); // Ouest
	
	// Dessin des caractères cardinaux (NSEO)
        //: Première lettre de Sud
    paint.drawText(QPointF(center->rx()-4, center->ry()+rayon+20), QString( tr("S") ));
        //: Première lettre de Nord
    paint.drawText(QPointF(center->rx()-4, center->ry()-rayon-10), QString( tr("N") ));
        //: Première lettre de Est
    paint.drawText(QPointF(center->rx()+rayon+10, center->ry()+4), QString( tr("E") ));
        //: Première lettre de Ouest
    paint.drawText(QPointF(center->rx()-rayon-20, center->ry()+4), QString( tr("W") ));
}

void WindCircle::drawDir(){
    cleanBuffer();
	setDirection(angle);
	#if DEBUG_VENT
	cout << "angle de windcircle" << angle << endl;
	#endif
	QPainter paint(buffer);
	QPen pen(Qt::red); paint.setPen(pen);

	paint.drawLine(*center, *direction);
}

void WindCircle::cleanBuffer(){
    buffer->fill(1);
    drawCircle();
}

/*** Events ***/
void WindCircle::paintEvent(QPaintEvent* Qevent){
    QWidget::paintEvent(Qevent);
    QPainter paint(this);
	paint.drawImage(0, 0, *buffer);
}

void WindCircle::resizeEvent(QResizeEvent* event){
    if (!buffer->isNull()){
		delete(buffer);
	}
	if(!center)
		center = new QPointF();
	
	buffer = new QImage(event->size().width(), event->size().height(), QImage::Format_ARGB32);

	center->setX(event->size().width()/2);
	center->setY(event->size().height()/2);
	setDirection(angle);
  	drawCircle();
	drawDir();
}

void WindCircle::mousePressEvent(QMouseEvent* event)
{
	float param = event->x()-center->x();
	float rayon = height()/2.0;
	param = param/ rayon;
	float result = atan((event->y()-center->y())/(event->x()-center->x()))*180.0 / PI;

	if(param<=0){
		result += 180;
	}

	#if DEBUG_VAR
	cout << "Taille de windcircle : " << width() << " ; " << height() << endl;
	cout << "Angle de windcircle : " << angle << endl;
	cout << "Angle résultat ? : " << (int)result << endl;
	#endif
    updateAngle((int)result);
}
