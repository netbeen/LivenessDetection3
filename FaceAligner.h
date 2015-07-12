#ifndef FACEALIGNER_H
#define FACEALIGNER_H

#include <opencv2/opencv.hpp>
#include <ESR.hpp>
#include <QObject>
#include <QThread>

class FaceAligner : public QObject
{
    Q_OBJECT
public:
    static FaceAligner* getInstance();
    cv::Mat_<double> getCurrentShape();

private:
    static FaceAligner* ptr2FaceAligner;
    FaceAligner();
    ~FaceAligner();
    const int initial_number;
    const int landmarkNum;
    ShapeRegressor regressor;
    cv::Mat_<double> currentShape;

    cv::Mat grayImg;
    BoundingBox boundingBox;

signals:
    void alignmentCompete(cv::Mat_<double>);

public slots:
    void doAlignment(cv::Mat grayImg, BoundingBox boundingBox);
};

#endif // FACEALIGNER_H
