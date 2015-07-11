#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <BoundingBox.h>

class Utils
{
public:
    static void drawRect(cv::Mat& input, BoundingBox& boundingBox);
    static void drawRect(cv::Mat &input, std::vector<cv::Rect>& eyesRects, int offsetX, int offsetY);
    static void drawPoint(cv::Mat& input, cv::Mat_<double>& shape);
    static void randomizeVector(std::vector<std::string>& inputVector);

private:
    Utils();
};

#endif // UTILS_H
