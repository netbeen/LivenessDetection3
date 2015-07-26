#include "YawAnalyser.h"

YawAnalyser::YawAnalyser():totalProgressTimeMS(3000)
{
    std::cout << "YawAnalyser constructor start" << std::endl;
    //webcamCapture = WebcamCapture::getInstance();
    //faceDetector = new FaceDetector();          //faceDetector对象
    //faceAligner = FaceAligner::getInstance();
    //QObject::connect(this,SIGNAL(doAlignment(cv::Mat,BoundingBox)),faceAligner,SLOT(doAlignment(cv::Mat,BoundingBox)));
    //QObject::connect(faceAligner,SIGNAL(alignmentCompete(cv::Mat_<double>)),this,SLOT(receiveNewAlignment(cv::Mat_<double>)));
    std::cout << "YawAnalyser constructor over" << std::endl;
}

void YawAnalyser::start(){
    std::cout << "YawAnalyser start() start" << std::endl;
    progressTimer = new QTimer();
    QObject::connect(this->progressTimer,SIGNAL(timeout()),this,SLOT(timeout()));      //绑定计时器事件

    //QObject::connect(this,SIGNAL(webcamStart()),webcamCapture,SLOT(start()));
    //QObject::connect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));
    //emit this->webcamStart();

    std::cout << "YawAnalyser at " << QThread::currentThreadId()<< std::endl;
    std::cout << "YawAnalyser start() over" << std::endl;

    progressTimer->start(totalProgressTimeMS);

}

void YawAnalyser::timeout(){
    progressTimer->stop();
    //QObject::disconnect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));
    //QObject::disconnect(this,SIGNAL(webcamStart()),webcamCapture,SLOT(start()));
    std::cout << "YawAnalyser Time out!"<<std::endl;
    emit this->done(true);
}

void YawAnalyser::receiveNewFrame(cv::Mat newFrame){
    cv::cvtColor(newFrame,this->grayImage,cv::COLOR_BGR2GRAY);
    /*if (this->faceDetector->detect(this->grayImage,5, this->faceBoundingBox)) {       //调用FaceDetector，如果检测到脸
        emit this->doAlignment(this->grayImage, this->faceBoundingBox);

        Utils::drawRect(this->grayImage,this->faceBoundingBox);
        if(this->isCurrentAlignmentValid == true){
            Utils::drawPoint(this->grayImage,this->currentAlignment);
            if((this->currentAlignment(107,1) - this->currentAlignment(93,1)) > (this->currentAlignment(93,1) - this->currentAlignment(65,1)) * 3 ){
                this->success();
            }
        }
    }else{                                                  //如果没检测到脸

    }*/
    cv::moveWindow("Yaw",200,160);
    cv::imshow("Yaw", grayImage);
}

/*void YawAnalyser::receiveNewOpticalFlow(cv::Mat_<double>){

}*/
