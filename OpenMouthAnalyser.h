#ifndef OpenMouthAnalyser_h
#define OpenMouthAnalyser_H

#include <Analyser.h>
#include <WebcamCapture.h>
#include <FaceDetector.h>
#include <Utils.h>
#include <QTimer>
#include <FaceAligner.h>

class OpenMouthAnalyser : public Analyser
{
    Q_OBJECT
public:
    OpenMouthAnalyser();

private:
    WebcamCapture* webcamCapture;
    FaceDetector* faceDetector;
    cv::Mat grayImage;
    BoundingBox faceBoundingBox;
    QTimer* timeoutTimer;
    const int timeoutTimeMs;
    FaceAligner* faceAligner;
    void success();
    cv::Mat_<double> currentAlignment;
    bool isCurrentAlignmentValid;

public slots:
    virtual void start();
    void receiveNewFrame(cv::Mat newFrame);
    void receiveNewAlignment(cv::Mat_<double>);

private slots:
    void timeout();

signals:
    void done(bool result);
    void webcamStart();
    void doAlignment(cv::Mat grayImage, BoundingBox boundingBox);
};

#endif // OpenMouthAnalyser_H
