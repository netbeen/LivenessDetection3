#include "EyeDetector.h"

EyeDetector::EyeDetector()
{
    cv::String eyeDetectorPath = "/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml";
    this->cascadeClassifier.load(eyeDetectorPath);
}

EyeDetector::~EyeDetector()
{

}

bool EyeDetector::detect(cv::Mat& grayImg, std::vector<cv::Rect>& eyesRects) {
    this->cascadeClassifier.detectMultiScale(grayImg, eyesRects,2,2, 0 | cv::CASCADE_DO_CANNY_PRUNING | cv::CASCADE_DO_ROUGH_SEARCH | cv::CASCADE_SCALE_IMAGE, cv::Size(40, 40));
    if (eyesRects.empty()) {
        return false;
    } else {
        return true;
    }
}
