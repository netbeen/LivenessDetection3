#include "OpenMouthAnalyser.h"
#include <QThread>

OpenMouthAnalyser::OpenMouthAnalyser():timeoutTimeMs(10000),isCurrentAlignmentValid(false),openMouthThreshold(2.5)
{
    webcamCapture = WebcamCapture::getInstance();
    faceDetector = new FaceDetector();          //faceDetector对象
    faceAligner = FaceAligner::getInstance();
    QObject::connect(this,SIGNAL(doAlignment(cv::Mat,BoundingBox)),faceAligner,SLOT(doAlignment(cv::Mat,BoundingBox)));
    QObject::connect(faceAligner,SIGNAL(alignmentCompete(cv::Mat_<double>)),this,SLOT(receiveNewAlignment(cv::Mat_<double>)));
}

void OpenMouthAnalyser::start(){
    timeoutTimer = new QTimer();
    QObject::connect(timeoutTimer,SIGNAL(timeout()),this,SLOT(timeout()));      //绑定计时器事件
    std::cout << "OpenMouthAnalyser at " << QThread::currentThreadId()<< std::endl;
    QObject::connect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));
    timeoutTimer->start(timeoutTimeMs);

}

void OpenMouthAnalyser::receiveNewFrame(cv::Mat newFrame){
    cv::cvtColor(newFrame,this->grayImage,cv::COLOR_BGR2GRAY);
    if (this->faceDetector->detect(this->grayImage,5, this->faceBoundingBox)) {       //调用FaceDetector，如果检测到脸
        emit this->doAlignment(this->grayImage, this->faceBoundingBox);

        Utils::drawRect(this->grayImage,this->faceBoundingBox);
        if(this->isCurrentAlignmentValid == true){
            Utils::drawPoint(this->grayImage,this->currentAlignment);
            if((this->currentAlignment(107,1) - this->currentAlignment(93,1)) > (this->currentAlignment(93,1) - this->currentAlignment(65,1)) * this->openMouthThreshold ){
                this->success();
            }
        }
    }else{                                                  //如果没检测到脸

    }
    //cv::moveWindow("OpenMouthAnalyser",200,160);
    //cv::imshow("OpenMouthAnalyser", grayImage);
}

void OpenMouthAnalyser::timeout(){

    timeoutTimer->stop();
    QObject::disconnect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));   //解绑接收摄像头事件
    //cv::destroyAllWindows();
    std::cout << "OpenMouthAnalyser Time out!"<<std::endl;
    emit this->done(false);
}

void OpenMouthAnalyser::success(){

    timeoutTimer->stop();
    QObject::disconnect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));   //解绑接收摄像头事件
    cv::destroyAllWindows();
    std::cout << "OpenMouthAnalyser success!"<<std::endl;
    emit this->done(true);
}

void OpenMouthAnalyser::receiveNewAlignment(cv::Mat_<double> newAlignment){
    this->currentAlignment = newAlignment;
    this->isCurrentAlignmentValid = true;
}
