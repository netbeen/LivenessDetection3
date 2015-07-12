#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <AnalyserFactory.h>
#include <vector>
#include <QThread>
#include <Utils.h>

class Controller : public QObject
{
    Q_OBJECT
public:
    static Controller* getInstance();

private:
    Controller();
    static Controller* ptr2Controller;
    AnalyserFactory* analyserFactory;
    std::vector<std::string> analyserOrder;     //用于保存测试顺序的vector
    size_t currentAnalyseIndex;
    std::vector<Analyser*> analyserVector;
    QThread* analyserThread;
    QThread* webcamThread;
    QThread* faceAlignerThread;
    WebcamCapture* webcamCapture;
    FaceAligner* faceAligner;

signals:
    void startNextAnalyserSignal();
    void analyserStartSignal();

public slots:
    void start();
    void startNextAnalyserSlot();
    void receiveAnalyserResultSlot(bool result);

};

#endif // CONTROLLER_H
