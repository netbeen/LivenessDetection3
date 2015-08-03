#ifndef ANALYSER_H
#define ANALYSER_H

#include <QObject>

class Analyser : public QObject
{
    Q_OBJECT
public:
    Analyser();

public slots:
    virtual void start()=0;

signals:
    void updateSlider(int percentage);
};

#endif // ANALYSER_H
