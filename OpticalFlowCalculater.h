#ifndef OPTICALFLOWCALCULATER_H
#define OPTICALFLOWCALCULATER_H

#include <QObject>
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>

class OpticalFlowCalculater : public QObject
{
    Q_OBJECT
public:
    static OpticalFlowCalculater* getInstance();

private:
    OpticalFlowCalculater();
    static OpticalFlowCalculater* ptr2OpticalFlowCalculater;
    cv::Mat previousFrame;
    cv::Mat flow;

public slots:
    void doCalc(cv::Mat grayImg);

signals:
    void calcCompete(bool isVaild, cv::Mat flow);
};

#endif // OPTICALFLOWCALCULATER_H
