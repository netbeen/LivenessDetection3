#include "Utils.h"

Utils::Utils()
{

}

void Utils::drawPoint(cv::Mat& input, cv::Mat_<double>& shape){
    for (int i = 0; i < shape.rows; i++) {
        circle(input, cv::Point2d(shape(i, 0), shape(i, 1)), 3, cv::Scalar(0, 255, 0), -1, 8, 0);
    }
}

void Utils::drawRect(cv::Mat& input, BoundingBox& boundingBox){
    cv::rectangle(input, boundingBox.returnRect(), cv::Scalar(0, 255, 255), 3, 8, 0);
    return;
}

void Utils::drawRect(cv::Mat &input, std::vector<cv::Rect>& eyesRects, int offsetX, int offsetY){
    for(cv::Rect& rectElem : eyesRects){
        rectElem.x +=offsetX;        //加上人脸框X轴的偏移
        rectElem.y +=offsetY;        //加上人脸框Y轴的偏移
        cv::rectangle(input, rectElem, cv::Scalar(0, 255, 255), 3, 8, 0);
    }
}

void Utils::randomizeVector(std::vector<std::string>& inputVector){
    const int swapCount = inputVector.size();
    for(int i = 0; i < swapCount; ++i){
        std::swap(inputVector.at(random()%inputVector.size()), inputVector.at(random()%inputVector.size()));
    }
}
