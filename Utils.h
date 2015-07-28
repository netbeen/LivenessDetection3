#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <BoundingBox.h>
#include <cmath>

class Utils
{
public:
    static void drawRect(cv::Mat& input, BoundingBox& boundingBox);
    static void drawRect(cv::Mat &input, std::vector<cv::Rect>& eyesRects, int offsetX, int offsetY);
    static void drawPoint(cv::Mat& input, cv::Mat_<double>& shape);
    static void randomizeVector(std::vector<std::string>& inputVector);
    static double calculateStandardDeviation(const std::vector<double>& v);
    static double calculatePearsonCorrelation(const std::vector<double>& v_1, const std::vector<double>& v_2);

    static cv::Mat_<double> getMeanShape(const std::vector<cv::Mat_<double> >& shapes, const std::vector<BoundingBox>& bounding_box);
    static cv::Mat_<double> projectShape(const cv::Mat_<double>& shape, const BoundingBox& bounding_box);
    static cv::Mat_<double> reProjectShape(const cv::Mat_<double>& shape, const BoundingBox& bounding_box);
    static void SimilarityTransform(const cv::Mat_<double>& shape1, const cv::Mat_<double>& shape2, cv::Mat_<double>& rotation, double& scale);
    static double calculate_covariance(const std::vector<double>& v_1, const std::vector<double>& v_2);

private:
    Utils();
};

#endif // UTILS_H
