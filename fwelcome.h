#ifndef FWELCOME_H
#define FWELCOME_H
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QSlider>

class Fwelcome : public QDialog
{
Q_OBJECT
private:
	QPushButton* valid_btn;
	QLineEdit* haut_edit;
	QLineEdit* larg_edit;
	QLabel* p_value;
	QLabel* c_value;
	float proba;
	float coef_brulure;
	
public:
	Fwelcome(QWidget* parent);
	virtual ~Fwelcome();
public slots:
	void set_proba(int x);
	void set_coef(int x);
};

#endif // FWELCOME_H
