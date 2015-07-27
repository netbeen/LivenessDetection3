#ifndef BLINKANALYSER_H
#define BLINKANALYSER_H

#include <Analyser.h>
#include <WebcamCapture.h>
#include <EyeDetector.h>
#include <FaceDetector.h>
#include <Utils.h>
#include <QTimer>

class BlinkAnalyser : public Analyser
{
    Q_OBJECT
public:
    BlinkAnalyser();

private:
    WebcamCapture* webcamCapture;
    int blinkCount;
    EyeDetector* eyeDetector;
    FaceDetector* faceDetector;
    cv::Mat grayImage;
    BoundingBox faceBoundingBox;
    std::vector<cv::Rect> eyesRects;
    cv::Mat upperImage;
    bool isEyesOpen;
    const int blinkThreshold;
    QTimer* timeoutTimer;
    void success();
    const int timeoutTimeMs;

public slots:
    virtual void start();
    void receiveNewFrame(cv::Mat newFrame);

private slots:
    void timeout();

signals:
    void done(bool result);
};

#endif // BLINKANALYSER_H
