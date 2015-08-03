#include "YawAnalyser.h"
#include <cmath>

//构造函数
YawAnalyser::YawAnalyser():totalProgressTimeMS(5000),isCurrentAlignmentValid(false),isOpticalFlowCalculaterBusy(false),sliderPhase(0)
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
    this->phaseAngle = cv::Mat(600,800, CV_32FC1);
    this->zoneMap = cv::Mat(600,800,CV_8UC1);
}

//启动入口函数
void YawAnalyser::start(){
    std::cout << "YawAnalyser at " << QThread::currentThreadId()<< std::endl;
    QObject::connect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));  //绑定接收摄像头事件

    this->progressTimer = new QTimer();
    QObject::connect(this->progressTimer,SIGNAL(timeout()),this,SLOT(timeout()));      //绑定计时器事件
    this->updateSliderTimer = new QTimer();
    QObject::connect(this->updateSliderTimer,SIGNAL(timeout()),this,SLOT(updateSliderTimeout()));
    this->progressTimer->start(totalProgressTimeMS);
    this->updateSliderTimer->start(totalProgressTimeMS/1000);
}

void YawAnalyser::updateSliderTimeout(){

    float currentY = 50*(sin(this->sliderPhase/1000*4*3.1415926)+1);
    this->sliderPhase++;
    emit this->updateSlider(currentY);
}

//摇头测试结束，开始计算结果
void YawAnalyser::timeout(){
    this->progressTimer->stop();
    this->updateSliderTimer->stop();
    QObject::disconnect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(receiveNewFrame(cv::Mat)));   //解绑接收摄像头事件
    QObject::disconnect(this,SIGNAL(doCalcOpticalFlow(cv::Mat)),this->opticalFlowCalculater,SLOT(doCalc(cv::Mat)));     //解绑光流计算事件
    QObject::disconnect(opticalFlowCalculater,SIGNAL(calcCompete(bool,cv::Mat)),this,SLOT(receiveNewOpticalFlow(bool,cv::Mat)));    //解绑接受光流事件
    //cv::destroyAllWindows();
    std::cout << "YawAnalyser Time out!"<<std::endl;

    /////////////////////
    std::cout << "捕获到光流总帧数：" <<this->leftBackgroundNormalVector.size() <<std::endl;
    std::cout << "左模" << Utils::calculatePearsonCorrelation(this->faceNormalVector,this->leftBackgroundNormalVector) <<std::endl;
    std::cout << "右模"<< Utils::calculatePearsonCorrelation(this->faceNormalVector,this->rightBackgroundNormalVector) <<std::endl;
    std::cout << "左相位"<< Utils::calculatePearsonCorrelation(this->facePhaseVector,this->leftBackgroundPhaseVector) <<std::endl;
    std::cout << "右相位"<< Utils::calculatePearsonCorrelation(this->facePhaseVector,this->rightBackgroundPhaseVector) <<std::endl;

    emit this->done(true);
}

//static int count = 0;

//接收新图像的slot
void YawAnalyser::receiveNewFrame(cv::Mat newFrame){

    cv::cvtColor(newFrame,this->grayImage,cv::COLOR_BGR2GRAY);
    this->imageToDisplay = this->grayImage.clone();
    if (this->faceDetector->detect(this->grayImage,5, this->faceBoundingBox)) {       //调用FaceDetector，如果检测到脸

        emit this->doAlignment(this->grayImage, this->faceBoundingBox); //进行人脸对齐

        if(this->isCurrentAlignmentValid == true){

            if(isOpticalFlowCalculaterBusy){
                //光流计算器繁忙，跳过本轮
            }else{
                isOpticalFlowCalculaterBusy = true;
                emit this->doCalcOpticalFlow(this->grayImage);  //进行光流计算
                this->calculateZoneMap();
            }

            Utils::drawPoint(this->imageToDisplay,this->currentAlignment);
        }
        Utils::drawRect(this->imageToDisplay,this->faceBoundingBox); //绘制人脸包围盒
    }else{                                                  //如果没检测到脸

    }
    //std::cout << count++ << std::endl;
    //cv::moveWindow("Yaw",0,160);
    //cv::imshow("Yaw", imageToDisplay);
}

//接收新对齐的slot
void YawAnalyser::receiveNewAlignment(cv::Mat_<double> newAlignment){
    this->currentAlignment = newAlignment;
    this->isCurrentAlignmentValid = true;
}

//接收新光流的slot
void YawAnalyser::receiveNewOpticalFlow(bool isOpticalFlowVaild, cv::Mat newOpticalFlow){

    if(isOpticalFlowVaild){

        //displayZoneMap();   //DEBUG：显示区域映射图

        this->currentOpticalFlow = newOpticalFlow;
        this->separateNromAndAngle();
        this->recordIntoVectors();

        /*cv::moveWindow("norm",600,160);
        cv::imshow("norm", this->norm);
        cv::moveWindow("phaseAngle",600,760);
        cv::imshow("phaseAngle", this->phaseAngle);*/
    }else{

    }
    this->isOpticalFlowCalculaterBusy = false;
}

//从光流中分离法向量与相位角
void YawAnalyser::separateNromAndAngle(){
    for(int rowIndex = 0; rowIndex < this->currentOpticalFlow.rows; ++rowIndex){
        for(int columnIndex = 0; columnIndex < this->currentOpticalFlow.cols; ++columnIndex){
            cv::Vec2f flow_at_point = this->currentOpticalFlow.at<cv::Vec2f>(rowIndex, columnIndex);
            float normValue = std::sqrt(std::pow(flow_at_point[0],2)+std::pow(flow_at_point[1],2));
            this->norm.at<float>(rowIndex, columnIndex)=normValue;
            float angleValue = atan2(flow_at_point[0],flow_at_point[1]);
            //uchar angleValueUchar = static_cast<uchar>(((angleValue/3.1415926*180)/180*127)+127);		//将float的-pi到pi的值映射到0-255的uchar来显示
            //this->phaseAngle.at<uchar>(rowIndex, columnIndex)=angleValueUchar;
            this->phaseAngle.at<float>(rowIndex, columnIndex)=angleValue;
        }
    }
}

//计算区域映射图: opencv坐标系中(屏幕上方为X轴，屏幕左侧为Y轴)
void  YawAnalyser::calculateZoneMap(){
    for(int rowIndex = 0; rowIndex < this->grayImage.rows; ++rowIndex){
        for(int columnIndex = 0; columnIndex < this->grayImage.cols; ++columnIndex){
            if(columnIndex > this->currentAlignment.at<double>(0,0) && columnIndex < this->currentAlignment.at<double>(40,0)
               && rowIndex > this->faceBoundingBox.startY && rowIndex < this->faceBoundingBox.startY+this->faceBoundingBox.height
               && this->isSinglePointInFace(rowIndex,columnIndex)){
                this->zoneMap.at<int>(rowIndex,columnIndex)=zone::face;
            }else if(columnIndex > this->currentAlignment.at<double>(0,0)-this->faceBoundingBox.width/2 && columnIndex < this->faceBoundingBox.startX && columnIndex < this->currentAlignment.at<double>(0,0) && rowIndex < this->faceBoundingBox.startY+this->faceBoundingBox.height){
                this->zoneMap.at<int>(rowIndex,columnIndex)=zone::leftBackground;
            }else if(columnIndex < this->currentAlignment.at<double>(40,0)+this->faceBoundingBox.width/2  && columnIndex > this->faceBoundingBox.startX+this->faceBoundingBox.width && columnIndex > this->currentAlignment.at<double>(40,0) && rowIndex < this->faceBoundingBox.startY+this->faceBoundingBox.height){
                this->zoneMap.at<int>(rowIndex,columnIndex)=zone::rightBackground;
            }
            else{
                this->zoneMap.at<int>(rowIndex,columnIndex)=zone::noArea;
            }
        }
    }
}


bool YawAnalyser::isSinglePointInFace(int rowIndex, int columnIndex){
    if(columnIndex <= this->currentAlignment.at<double>(20,0)){
        for(int keypointIndex = 0; keypointIndex <= 20; keypointIndex++){
            if(rowIndex > this->currentAlignment.at<double>(keypointIndex,1)){
                continue;
            }else{
                if(columnIndex < this->currentAlignment.at<double>(keypointIndex,0)){
                    return false;
                }else{
                    return true;
                }
            }
        }
        return false;
    }else{
        for(int keypointIndex = 40; keypointIndex >= 20; keypointIndex--){
            if(rowIndex > this->currentAlignment.at<double>(keypointIndex,1)){
                continue;
            }else{
                if(columnIndex > this->currentAlignment.at<double>(keypointIndex,0)){
                    return false;
                }else{
                    return true;
                }
            }
        }
        return false;
    }
}

//DEBUG: 显示区域映射图
void YawAnalyser::displayZoneMap(){
    cv::Vec3b blue = {255,0,0};
    cv::Vec3b green = {0,255,0};
    cv::Vec3b green2 = {150,200,150};
    cv::Vec3b red = {0,0,255};

    cv::Mat zoneMapToDisplay = cv::Mat(600,800, CV_8UC3);
    for(int rowIndex = 0; rowIndex < zoneMapToDisplay.rows; ++rowIndex){
        for(int columnIndex = 0; columnIndex < zoneMapToDisplay.cols; ++columnIndex){
            if(this->zoneMap.at<int>(rowIndex,columnIndex) == zone::face){
                zoneMapToDisplay.at<cv::Vec3b>(rowIndex, columnIndex)=blue;
            }else if(this->zoneMap.at<int>(rowIndex,columnIndex) == zone::leftBackground){
                zoneMapToDisplay.at<cv::Vec3b>(rowIndex, columnIndex)=green;
            }else if(this->zoneMap.at<int>(rowIndex,columnIndex) == zone::rightBackground){
                zoneMapToDisplay.at<cv::Vec3b>(rowIndex, columnIndex)=green2;
            }
            else{
                zoneMapToDisplay.at<cv::Vec3b>(rowIndex, columnIndex)=red;
            }
        }
    }
    cv::moveWindow("ZoneMap",1200,160);
    cv::imshow("ZoneMap",zoneMapToDisplay);
}

//记录当前数据
void YawAnalyser::recordIntoVectors(){
    float sumNormFace = 0;
    float sumNormLeftground = 0;
    float sumNormRightground = 0;

    float sumPhaseFace = 0;
    float sumPhaseLeftground = 0;
    float sumPhaseRightground = 0;

    int countFace = 0;
    int countLeftground = 0;
    int countRightground = 0;
    for(int rowIndex = 0; rowIndex < this->norm.rows; ++rowIndex){
        for(int columnIndex = 0; columnIndex < this->norm.cols; ++columnIndex){
            if(this->zoneMap.at<int>(rowIndex,columnIndex) == zone::face){
                countFace++;
                sumNormFace += norm.at<float>(rowIndex, columnIndex);
                sumPhaseFace += phaseAngle.at<float>(rowIndex,columnIndex);
            }else if(this->zoneMap.at<int>(rowIndex,columnIndex) == zone::leftBackground){
                countLeftground++;
                sumNormLeftground += norm.at<float>(rowIndex, columnIndex);
                sumPhaseLeftground += phaseAngle.at<float>(rowIndex,columnIndex);
            }else if(this->zoneMap.at<int>(rowIndex,columnIndex) == zone::rightBackground){
                countRightground++;
                sumNormRightground += norm.at<float>(rowIndex, columnIndex);
                sumPhaseRightground += phaseAngle.at<float>(rowIndex,columnIndex);
            }
        }
    }
    float expectionNormFace = sumNormFace/countFace;
    float expectionNormLeftground = sumNormLeftground/countLeftground;
    float expectionNormRightground = sumNormRightground/countRightground;

    this->faceNormalVector.push_back(expectionNormFace);
    this->leftBackgroundNormalVector.push_back(expectionNormLeftground);
    this->rightBackgroundNormalVector.push_back(expectionNormRightground);

    this->facePhaseVector.push_back(sumPhaseFace/countFace);
    this->leftBackgroundPhaseVector.push_back(sumPhaseLeftground/countLeftground);
    this->rightBackgroundPhaseVector.push_back(sumPhaseRightground/countRightground);

}
