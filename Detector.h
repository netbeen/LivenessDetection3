#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/opencv.hpp>
#include <BoundingBox.h>
#include <vector>

class Detector
{
public:
    Detector();
    virtual ~Detector();
protected:
    cv::CascadeClassifier cascadeClassifier;
};

#endif // DETECTOR_H
