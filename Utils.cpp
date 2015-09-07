#include "Utils.h"

using namespace std;
using namespace cv;

Utils::Utils()
{

}

//在input图像上绘制一系列点
void Utils::drawPoint(cv::Mat& input, cv::Mat_<double>& shape){
    for (int i = 0; i < shape.rows; i++) {
        circle(input, cv::Point2d(shape(i, 0), shape(i, 1)), 3, cv::Scalar(0, 255, 0), -1, 8, 0);
    }
}

//在input图像上绘制矩形
void Utils::drawRect(cv::Mat& input, BoundingBox& boundingBox){
    cv::rectangle(input, boundingBox.returnRect(), cv::Scalar(0, 255, 255), 3, 8, 0);
    return;
}

//在input上绘制有偏移的矩形
void Utils::drawRect(cv::Mat &input, std::vector<cv::Rect>& eyesRects, int offsetX, int offsetY){
    for(cv::Rect& rectElem : eyesRects){
        rectElem.x +=offsetX;        //加上人脸框X轴的偏移
        rectElem.y +=offsetY;        //加上人脸框Y轴的偏移
        cv::rectangle(input, rectElem, cv::Scalar(0, 255, 255), 3, 8, 0);
    }
}

//随机化vector内的元素位置
void Utils::randomizeVector(std::vector<std::string>& inputVector){
    const int swapCount = inputVector.size();
    for(int i = 0; i < swapCount; ++i){
        std::swap(inputVector.at(random()%inputVector.size()), inputVector.at(random()%inputVector.size()));
    }
}



//从所有的shape中得到mean shape
Mat_<double> Utils::getMeanShape(const vector<Mat_<double> >& shapes, const vector<BoundingBox>& bounding_box) {
    cout << "Starting GetMeanShape..." <<endl;
    Mat_<double> result = Mat::zeros(shapes[0].rows, 2, CV_64FC1);
    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        result = result + projectShape(shapes[i], bounding_box[i]);
    }
    result = 1.0 / shapes.size() * result;
    return result;
}

//
Mat_<double> Utils::projectShape(const Mat_<double>& shape, const BoundingBox& bounding_box) {
    Mat_<double> temp(shape.rows, 2);
    for (int j = 0; j < shape.rows; j++) {
        temp(j, 0) = (shape(j, 0) - bounding_box.centerX) / (bounding_box.width / 2.0);
        temp(j, 1) = (shape(j, 1) - bounding_box.centerY) / (bounding_box.height / 2.0);
    }
    return temp;
}

//
Mat_<double> Utils::reProjectShape(const Mat_<double>& shape, const BoundingBox& bounding_box) {
    Mat_<double> temp(shape.rows, 2);
    for (int j = 0; j < shape.rows; j++) {
        temp(j, 0) = (shape(j, 0) * bounding_box.width / 2.0 + bounding_box.centerX);
        temp(j, 1) = (shape(j, 1) * bounding_box.height / 2.0 + bounding_box.centerY);
    }
    return temp;
}

void Utils::SimilarityTransform(const Mat_<double>& shape1, const Mat_<double>& shape2, Mat_<double>& rotation, double& scale) {
    rotation = Mat::zeros(2, 2, CV_64FC1);
    scale = 0;

    // center the data
    double center_x_1 = 0;
    double center_y_1 = 0;
    double center_x_2 = 0;
    double center_y_2 = 0;
    for (int i = 0; i < shape1.rows; i++) {
        center_x_1 += shape1(i, 0);
        center_y_1 += shape1(i, 1);
        center_x_2 += shape2(i, 0);
        center_y_2 += shape2(i, 1);
    }
    center_x_1 /= shape1.rows;
    center_y_1 /= shape1.rows;
    center_x_2 /= shape2.rows;
    center_y_2 /= shape2.rows;

    Mat_<double> temp1 = shape1.clone();
    Mat_<double> temp2 = shape2.clone();
    for (int i = 0; i < shape1.rows; i++) {
        temp1(i, 0) -= center_x_1;
        temp1(i, 1) -= center_y_1;
        temp2(i, 0) -= center_x_2;
        temp2(i, 1) -= center_y_2;
    }

    Mat_<double> covariance1, covariance2;	//covariance = 协方差
    Mat_<double> mean1, mean2;
    // calculate covariance matrix
    cv::calcCovarMatrix(temp1, covariance1, mean1, CV_COVAR_COLS);	//计算temp1的协方差
    cv::calcCovarMatrix(temp2, covariance2, mean2, CV_COVAR_COLS);

    double s1 = sqrt(norm(covariance1));
    double s2 = sqrt(norm(covariance2));
    scale = s1 / s2;
    temp1 = 1.0 / s1 * temp1;
    temp2 = 1.0 / s2 * temp2;

    double num = 0;
    double den = 0;
    for (int i = 0; i < shape1.rows; i++) {
        num = num + temp1(i, 1) * temp2(i, 0) - temp1(i, 0) * temp2(i, 1);
        den = den + temp1(i, 0) * temp2(i, 0) + temp1(i, 1) * temp2(i, 1);
    }

    double norm = sqrt(num * num + den * den);
    double sin_theta = num / norm;
    double cos_theta = den / norm;
    rotation(0, 0) = cos_theta;
    rotation(0, 1) = -sin_theta;
    rotation(1, 0) = sin_theta;
    rotation(1, 1) = cos_theta;
}

//计算协方差
double Utils::calculate_covariance(const vector<double>& v_1, const vector<double>& v_2) {
    assert(v_1.size() == v_2.size());
    assert(v_1.size() != 0);
    double sum_1 = 0;
    double sum_2 = 0;
    double exp_1 = 0;
    double exp_2 = 0;
    double exp_3 = 0;
    for (int i = 0; i < static_cast<int>(v_1.size()); i++) {
        sum_1 += v_1[i];
        sum_2 += v_2[i];
    }
    exp_1 = sum_1 / v_1.size();
    exp_2 = sum_2 / v_2.size();
    for (int i = 0; i < static_cast<int>(v_1.size()); i++) {
        exp_3 = exp_3 + (v_1[i] - exp_1) * (v_2[i] - exp_2);
    }
    return exp_3 / v_1.size();
}

//计算标准差
double Utils::calculateStandardDeviation(const vector<double>& v) {
    assert(v.size() != 0);
    double sum = 0;
    double squareSum = 0;
    for(double elem : v){
        sum += elem;
        squareSum += elem*elem;
    }
    double expection = sum / v.size();
    double squareSumExpection = squareSum / v.size();
    return sqrt(squareSumExpection-(expection*expection));

}

//计算皮尔逊相关系数
double Utils::calculatePearsonCorrelation(const vector<double>& v_1, const vector<double>& v_2) {
    assert(v_1.size() == v_2.size());
    assert(v_1.size() != 0);
    double cov = Utils::calculate_covariance(v_1,v_2);
    double standardDeviation1 = Utils::calculateStandardDeviation(v_1);
    double standardDeviation2 = Utils::calculateStandardDeviation(v_2);
    return cov/(standardDeviation1*standardDeviation2);
}
