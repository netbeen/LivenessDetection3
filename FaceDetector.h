#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <Detector.h>

class FaceDetector : public Detector
{
public:
    FaceDetector();
    virtual ~FaceDetector();
    bool detect(cv::Mat& grayImg, double scale, BoundingBox& boundingBox);
};

#endif // FACEDETECTOR_H
