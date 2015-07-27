#include "YawAnalyser.h"
#include <cmath>

YawAnalyser::YawAnalyser():totalProgressTimeMS(3000),isCurrentAlignmentValid(false),isOpticalFlowCalculaterBusy(false)
{
    this->webcamCapture = WebcamCapture::getInstance();
    this->faceDetector = new FaceDetector();          //faceDetector对象
    faceAligner = FaceAligner::getInstance();
    QObject::connect(this,SIGNAL(doAlignment(cv::Mat,BoundingBox)),faceAligner,SLOT(doAlignment(cv::Mat,BoundingBox)));
    QObject::connect(faceAligner,SIGNAL(alignmentCompete(cv::Mat_<double>)),this,SLOT(receiveNewAlignment(cv::Mat_<double>)));

    this->opticalFlowCalculater = OpticalFlowCalculater::getInstance();
    QObject::connect(this,SIGNAL(doCalcOpticalFlow(cv::Mat)),this->opticalFlowCalculater,SLOT(doCalc(cv::Mat)));
    QObject::connect(opticalFlowCalculater,SIGNAL(calcCompete(bool,cv::Mat)),this,SLOT(receiveNewOpticalFlow(bool,cv::Mat)));

    this->norm = cv::Mat(600,800, CV_32FC1);//声明光流模的矩阵
    this->phaseAngle = cv::Mat(600,800, CV_8UC1);
}

void YawAnalyser::start(){
    std::cout << "YawAnalyser at " << QThread::currentThreadId()<< std::endl;
    QObject::connect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));  //绑定接收摄像头事件

    this->progressTimer = new QTimer();
    QObject::connect(this->progressTimer,SIGNAL(timeout()),this,SLOT(timeout()));      //绑定计时器事件
    this->progressTimer->start(totalProgressTimeMS);
}

void YawAnalyser::timeout(){
    this->progressTimer->stop();
    QObject::disconnect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));   //解绑接收摄像头事件
    QObject::disconnect(this,SIGNAL(doCalcOpticalFlow(cv::Mat)),this->opticalFlowCalculater,SLOT(doCalc(cv::Mat)));     //解绑光流计算事件
    QObject::disconnect(opticalFlowCalculater,SIGNAL(calcCompete(bool,cv::Mat)),this,SLOT(receiveNewOpticalFlow(bool,cv::Mat)));    //解绑接受光流事件
    std::cout << "YawAnalyser Time out!"<<std::endl;
    emit this->done(true);
}

void YawAnalyser::receiveNewFrame(cv::Mat newFrame){
    cv::cvtColor(newFrame,this->grayImage,cv::COLOR_BGR2GRAY);
    if (this->faceDetector->detect(this->grayImage,5, this->faceBoundingBox)) {       //调用FaceDetector，如果检测到脸
        emit this->doAlignment(this->grayImage, this->faceBoundingBox);


        if(this->isCurrentAlignmentValid == true){

            //do something
            if(isOpticalFlowCalculaterBusy){

            }else{
                isOpticalFlowCalculaterBusy = true;
                emit this->doCalcOpticalFlow(this->grayImage);
            }
            Utils::drawPoint(this->grayImage,this->currentAlignment);
        }
        Utils::drawRect(this->grayImage,this->faceBoundingBox);
    }else{                                                  //如果没检测到脸

    }
    cv::moveWindow("Yaw",200,160);
    cv::imshow("Yaw", grayImage);
}

void YawAnalyser::receiveNewAlignment(cv::Mat_<double> newAlignment){
    this->currentAlignment = newAlignment;
    this->isCurrentAlignmentValid = true;
}

void YawAnalyser::receiveNewOpticalFlow(bool isOpticalFlowVaild, cv::Mat newOpticalFlow){
    isOpticalFlowCalculaterBusy = false;
    if(isOpticalFlowVaild){
        std::cout << "isOpticalFlowVaild = true" << std::endl;
        this->currentOpticalFlow = newOpticalFlow;
        this->separateNromAndAngle();
        cv::moveWindow("norm",1000,160);
        cv::imshow("norm", this->norm);
    }else{
        std::cout << "isOpticalFlowVaild = false" << std::endl;
    }
}

void YawAnalyser::separateNromAndAngle(){
    std::cout << "YawAnalyser::separateNromAndAngle()" << std::endl;
    for(int rowIndex = 0; rowIndex < this->currentOpticalFlow.rows; ++rowIndex){
        for(int columnIndex = 0; columnIndex < this->currentOpticalFlow.cols; ++columnIndex){
            cv::Vec2f flow_at_point = this->currentOpticalFlow.at<cv::Vec2f>(rowIndex, columnIndex);
            float normValue = std::sqrt(std::pow(flow_at_point[0],2)+std::pow(flow_at_point[1],2));
            this->norm.at<float>(rowIndex, columnIndex)=normValue;
            float angleValue = atan2(flow_at_point[0],flow_at_point[1]);
            uchar angleValueUchar = static_cast<uchar>(((angleValue/3.1415926*180)/180*127)+127);		//将float的-pi到pi的值映射到0-255的uchar来显示
            this->phaseAngle.at<uchar>(rowIndex, columnIndex)=angleValueUchar;
        }
    }
}
