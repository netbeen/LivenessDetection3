#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <opencv2/opencv.hpp>

class BoundingBox
{
public:
    BoundingBox();
    ~BoundingBox();
    double startX;
    double startY;
    double width;
    double height;
    double centerX;
    double centerY;
    cv::Point_<double> getStartPoint();
    cv::Point_<double> getEndPoint();
    bool isInBoudingBox(cv::Point_<double> pt);
    cv::Rect returnRect();
    cv::Rect returnUpperRect();
};

#endif // BOUNDINGBOX_H
