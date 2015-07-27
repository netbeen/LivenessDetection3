#ifndef YAWANALYSER_H
#define YAWANALYSER_H

#include <Analyser.h>
#include <WebcamCapture.h>
#include <FaceDetector.h>
#include <FaceAligner.h>
#include <QTimer>
#include <Utils.h>
#include <OpticalFlowCalculater.h>

class YawAnalyser : public Analyser
{
    Q_OBJECT
public:
    YawAnalyser();

private:
    const int totalProgressTimeMS;
    QTimer* progressTimer;
    WebcamCapture* webcamCapture;
    FaceDetector* faceDetector;
    FaceAligner* faceAligner;
    cv::Mat grayImage;
    BoundingBox faceBoundingBox;
    OpticalFlowCalculater* opticalFlowCalculater;
    bool isOpticalFlowCalculaterBusy;

    bool isCurrentAlignmentValid;
    cv::Mat_<double> currentAlignment;
    cv::Mat currentOpticalFlow;
    void separateNromAndAngle();
    cv::Mat norm;
    cv::Mat phaseAngle;

public slots:
    virtual void start();
    void receiveNewFrame(cv::Mat newFrame);
    void receiveNewAlignment(cv::Mat_<double>);
    void receiveNewOpticalFlow(bool isOpticalFlowVaild, cv::Mat newOpticalFlow);

private slots:
    void timeout();

signals:
    void done(bool result);
    void doAlignment(cv::Mat grayImage, BoundingBox boundingBox);
    void doCalcOpticalFlow(cv::Mat);

};

#endif // YAWANALYSER_H
