#include "OpticalFlowCalculater.h"

OpticalFlowCalculater* OpticalFlowCalculater::ptr2OpticalFlowCalculater = nullptr;

OpticalFlowCalculater::OpticalFlowCalculater()
{
    this->previousFrame.release();

}

//单例模式的实例化函数
OpticalFlowCalculater* OpticalFlowCalculater::getInstance(){
    if(OpticalFlowCalculater::ptr2OpticalFlowCalculater == nullptr){
        OpticalFlowCalculater::ptr2OpticalFlowCalculater = new OpticalFlowCalculater();
    }
    return OpticalFlowCalculater::ptr2OpticalFlowCalculater;
}

//计算光流
void OpticalFlowCalculater::doCalc(cv::Mat grayImg){
    if(this->previousFrame.empty()){
        this->previousFrame = grayImg;
        emit this->calcCompete(false, this->flow);
    }else{
        calcOpticalFlowFarneback(this->previousFrame, grayImg, this->flow, 0.5, 3, 40, 3, 7, 1.5, 0);
        emit this->calcCompete(true, this->flow);
    }
}
