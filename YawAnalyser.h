#ifndef YAWANALYSER_H
#define YAWANALYSER_H

#include <Analyser.h>
#include <WebcamCapture.h>
#include <FaceDetector.h>
#include <FaceAligner.h>
#include <QTimer>
#include <Utils.h>

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

public slots:
    virtual void start();
    void receiveNewFrame(cv::Mat newFrame);
    //void receiveNewOpticalFlow(cv::Mat_<double>);

private slots:
    void timeout();

signals:
    virtual void done(bool result);
    void webcamStart();
};

#endif // YAWANALYSER_H
