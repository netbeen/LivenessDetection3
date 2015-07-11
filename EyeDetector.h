#ifndef EYEDETECTOR_H
#define EYEDETECTOR_H

#include <Detector.h>

class EyeDetector : public Detector
{
public:
    EyeDetector();
    virtual ~EyeDetector();
    bool detect(cv::Mat& grayImg, std::vector<cv::Rect>& eyesRects);
};

#endif // EYEDETECTOR_H
