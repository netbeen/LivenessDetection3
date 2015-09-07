#include "FaceDetector.h"

FaceDetector::FaceDetector()
{
    this->cascadeClassifier.load("/home/netbeen/workspace/20141015-ESR-HelenDatabase/data/haarcascade_frontalface_alt2.xml");
}

FaceDetector::~FaceDetector()
{

}

//人脸检测
bool FaceDetector::detect(cv::Mat& grayImg, double scale, BoundingBox& boundingBox){
    std::vector<cv::Rect> faces;
    cv::Mat smallImg = cv::Mat(cvRound(grayImg.rows / scale), cvRound(grayImg.cols / scale), CV_8UC1);
    resize(grayImg, smallImg, smallImg.size(), 0, 0, cv::INTER_CUBIC);
    cascadeClassifier.detectMultiScale(smallImg, faces, 1.1, 5, 0 | cv::CASCADE_DO_CANNY_PRUNING | cv::CASCADE_FIND_BIGGEST_OBJECT | cv::CASCADE_DO_ROUGH_SEARCH | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    if (faces.empty()) {
        return false;
    } else {
        boundingBox.startX = cvRound(faces[0].x * scale);
        boundingBox.startY = cvRound(faces[0].y * scale);
        boundingBox.width = cvRound((faces[0].width) * scale);
        boundingBox.height = cvRound((faces[0].height) * scale);
        boundingBox.centerX = boundingBox.startX + boundingBox.width / 2.0;
        boundingBox.centerY = boundingBox.startY+ boundingBox.height / 2.0;
        return true;
    }
}
