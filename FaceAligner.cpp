#include "FaceAligner.h"

FaceAligner* FaceAligner::ptr2FaceAligner = nullptr;

//单例模式创建函数
FaceAligner* FaceAligner::getInstance(){
    if(FaceAligner::ptr2FaceAligner == nullptr){
        FaceAligner::ptr2FaceAligner = new FaceAligner();
    }
    return FaceAligner::ptr2FaceAligner;
}

FaceAligner::FaceAligner():initial_number(20),landmarkNum(114)
{
    regressor = ShapeRegressor();
    regressor.load("/home/netbeen/LivenessDetectionTrainFile/model-Helen114-HaarAlt2-10-120.txt");
}

FaceAligner::~FaceAligner()
{

}


void FaceAligner::doAlignment(cv::Mat grayImg, BoundingBox boundingBox){
    this->currentShape =  regressor.predict(grayImg, boundingBox, this->initial_number);
    emit this->alignmentCompete(this->currentShape);
    return;
}

cv::Mat_<double> FaceAligner::getCurrentShape(){
    return currentShape;
}
