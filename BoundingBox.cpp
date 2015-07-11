#include "BoundingBox.h"

BoundingBox::~BoundingBox()
{
}

BoundingBox::BoundingBox(): startX(0),startY(0),width(0),height(0),centerX(0),centerY(0){
}

cv::Point_<double> BoundingBox::getStartPoint(){
    cv::Point_<double> point;
    point.x = startX;
    point.y = startY;
    return point;
}

cv::Point_<double> BoundingBox::getEndPoint(){
    cv::Point_<double> point;
    point.x = startX + width;
    point.y = startY + height;
    return point;
}

bool BoundingBox::isInBoudingBox(cv::Point_<double> pt){
    if(pt.x > startX && pt.x < (startX + width)){
        if(pt.y > startY && pt.y < (startY + height)){
            return true;
        }
    }
    return false;
}

cv::Rect BoundingBox::returnRect(){
    cv::Rect objectRect;
    objectRect.x = startX;
    objectRect.y = startY;
    objectRect.width = width;
    objectRect.height = height;
    return objectRect;
}

cv::Rect BoundingBox::returnUpperRect(){
    cv::Rect objectRect;
    objectRect.x = startX;
    objectRect.y = startY;
    objectRect.width = width;
    objectRect.height = height * 0.6;
    return objectRect;
}
