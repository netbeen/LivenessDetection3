#ifndef WEBCAMCAPTURE_H
#define WEBCAMCAPTURE_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <QObject>

class WebcamCapture:public QObject
{
    Q_OBJECT

public:
    static WebcamCapture* getInstance();

public slots:
    void start();

signals:
    void newImageCaptured(cv::Mat newImage);

private:
    static WebcamCapture* ptr2WebcamCapture;
    WebcamCapture();
    ~WebcamCapture();
    cv::VideoCapture cap;


};

#endif // WEBCAMCAPTURE_H
