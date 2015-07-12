/*
 * ESR.hpp
 *
 *  Created on: 2014年10月15日
 *      Author: netbeen
 */

#ifndef ESR_HPP_
#define ESR_HPP_

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <BoundingBox.h>
#include <Utils.h>

class Fern {

private:
	int fern_pixel_num_;
	int landmark_num_;
    cv::Mat_<int> selected_nearest_landmark_index_;
    cv::Mat_<double> threshold_;
    cv::Mat_<int> selected_pixel_index_;
    cv::Mat_<double> selected_pixel_locations_;
    std::vector<cv::Mat_<double> > bin_output_;
public:
    std::vector<cv::Mat_<double> > Train(const std::vector<std::vector<double> >& candidate_pixel_intensity, const cv::Mat_<double>& covariance, const cv::Mat_<double>& candidate_pixel_locations, const cv::Mat_<int>& nearest_landmark_index, const std::vector<cv::Mat_<double> >& regression_targets, int fern_pixel_num);
    cv::Mat_<double> predict(const cv::Mat_<uchar>& image, const cv::Mat_<double>& shape, const cv::Mat_<double>& rotation, const BoundingBox& bounding_box, double scale);
    void read(std::ifstream& fin);
    void Write(std::ofstream& fout);
};

class FernCascade {
public:
    std::vector<cv::Mat_<double> > train(const std::vector<cv::Mat_<uchar> >& images, const std::vector<cv::Mat_<double> >& current_shapes, const std::vector<cv::Mat_<double> >& ground_truth_shapes, const std::vector<BoundingBox> & bounding_box, const cv::Mat_<double>& mean_shape, int second_level_num, int candidate_pixel_num, int fern_pixel_num);
    cv::Mat_<double> predict(const cv::Mat_<uchar>& image, const BoundingBox& bounding_box, const cv::Mat_<double>& mean_shape, const cv::Mat_<double>& shape);
    void read(std::ifstream& fin);
    void Write(std::ofstream& fout);
private:
    std::vector<Fern> ferns_;
	int second_level_num_;
};

class ShapeRegressor {
public:
	ShapeRegressor();
    void train(const std::vector<cv::Mat_<uchar> >& images, const std::vector<cv::Mat_<double> >& ground_truth_shapes, const std::vector<BoundingBox>& bounding_box, int first_level_num, int second_level_num, int candidate_pixel_num, int fern_pixel_num, int initial_num);
    cv::Mat_<double> predict(const cv::Mat_<uchar>& image, const BoundingBox& bounding_box, int initial_num);
    void read(std::ifstream& fin);
    void Write(std::ofstream& fout);
    void load(std::string path);
    void save(std::string path);
private:
	int first_level_num_;
	int landmark_num_;
    std::vector<FernCascade> fernCascades;
    cv::Mat_<double> meanShape;
    std::vector<cv::Mat_<double> > training_shapes_;
    std::vector<BoundingBox> bounding_box_;
};

#endif /* ESR_HPP_ */
