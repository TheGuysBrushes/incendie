#ifndef AutomateFeu_H
#define AutomateFeu_H

#include <QtCore/QObject>

class AutomateFeu : public QObject
{
Q_OBJECT
public:
    AutomateFeu();
    virtual ~AutomateFeu();
private slots:
    void output();
};

#endif // AutomateFeu_H
