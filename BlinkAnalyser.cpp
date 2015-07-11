#include "BlinkAnalyser.h"
#include "QThread"

BlinkAnalyser::BlinkAnalyser():blinkCount(0),isEyesOpen(false),blinkThreshold(10)
{
    webcamCapture = WebcamCapture::getInstance();
    QObject::connect(this,SIGNAL(webcamStart()),webcamCapture,SLOT(start()));
    faceDetector = new FaceDetector();          //faceDetector对象
    eyeDetector = new EyeDetector();            //eyeDetector对象

}

void BlinkAnalyser::start(){
    timeoutTimer = new QTimer();
    QObject::connect(timeoutTimer,SIGNAL(timeout()),this,SLOT(timeout()));
    std::cout << "BlinkAnalyser at " << QThread::currentThreadId()<< std::endl;
    QObject::connect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));
    emit this->webcamStart();
    timeoutTimer->start(10000);

}

void BlinkAnalyser::receiveNewFrame(cv::Mat newFrame){
    cv::cvtColor(newFrame,this->grayImage,cv::COLOR_BGR2GRAY);
    if (this->faceDetector->detect(this->grayImage,5, this->faceBoundingBox)) {       //调用FaceDetector，如果检测到脸
        this->upperImage =  grayImage(this->faceBoundingBox.returnUpperRect());       //截取图像的上半部分（准备检测人眼）
        if(this->eyeDetector->detect(this->upperImage,this->eyesRects)){            //调用eyeDetector，如果检测到眼睛
            if(this->isEyesOpen == false){
                this->isEyesOpen = true;
                this->blinkCount++;
                //std::cout << this->blinkCount << std::endl;
            }
            Utils::drawRect(this->grayImage, eyesRects,this->faceBoundingBox.returnRect().x,this->faceBoundingBox.returnRect().y);
        }else{                                                                                                                                  //如果没检测到眼睛
            this->isEyesOpen = false;
        }
        Utils::drawRect(this->grayImage,this->faceBoundingBox);
    }else{                                                  //如果没检测到脸
        this->blinkCount = 0;
    }
    if(blinkCount > this->blinkThreshold){
        this->success();
    }
    cv::imshow("YY", grayImage);
}

void BlinkAnalyser::timeout(){
    timeoutTimer->stop();
    QObject::disconnect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));
    emit this->done(false);
    std::cout << "BlinkAnalyser Time out!"<<std::endl;

}

void BlinkAnalyser::success(){
    timeoutTimer->stop();
    QObject::disconnect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));
    emit this->done(true);
    std::cout << "BlinkAnalyser success!"<<std::endl;
}
