#ifndef ANALYSER_H
#define ANALYSER_H

#include <QObject>

class Analyser : public QObject
{
public:
    Analyser();

public slots:
    virtual void start()=0;
};

#endif // ANALYSER_H
