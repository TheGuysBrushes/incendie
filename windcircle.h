#ifndef WINDCIRCLE_H
#define WINDCIRCLE_H

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtCore/QPointF>
#include <QtGui/QPainter>
#include <QtGui/qevent.h>

/**
 * Cette classe correspond au widget d'affichage de la direction
 * @author Ugo
 * 
 */
class WindCircle : public QWidget
{
Q_OBJECT
private:
	QImage* buffer;
	QPointF* center;
	QPointF* direction;
	int angle;
	
public:
	WindCircle(int x);
	virtual ~WindCircle();
	void drawCircle();
	void drawDir();
	void effaceBuffer();
	
	int getAngle() const { return angle; };
	void setAngle(int x);
	void setDirection(int angle);
protected:
	void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent* event);
	
};

#endif // WINDCIRCLE_H
