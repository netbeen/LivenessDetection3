/*
 Author: Bi Sai
 Date: 2014/06/18
 This program is a reimplementation of algorithms in "Face Alignment by Explicit
 Shape Regression" by Cao et al.
 If you find any bugs, please email me: soundsilencebisai-at-gmail-dot-com

 Copyright (c) 2014 Bi Sai
 The MIT License (MIT)
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "ESR.hpp"

using namespace std;
using namespace cv;

ShapeRegressor::ShapeRegressor() {
	first_level_num_ = 0;
	landmark_num_ = 0;
}

/**
 * @param images gray scale images
 * @param ground_truth_shapes a vector of N*2 matrix, where N is the number of landmarks
 * @param bounding_box BoundingBox of faces
 * @param first_level_num number of first level regressors
 * @param second_level_num number of second level regressors
 * @param candidate_pixel_num number of pixels to be selected as features
 * @param fern_pixel_num number of pixel pairs in a fern
 * @param initial_num number of initial shapes for each input image
 */

void ShapeRegressor::read(ifstream& fin) {
	fin >> first_level_num_;					//第1行是第一层级联分类器的数量
	fin >> landmark_num_;					//第2行是landmark的数量

	meanShape = Mat::zeros(landmark_num_, 2, CV_64FC1);			//64位精度,在32位机上是double，64位机上是float
	for (int i = 0; i < landmark_num_; i++) {								//读入29个坐标（共58个数）作为初始的平均形状
		fin >> meanShape(i, 0) >> meanShape(i, 1);
	}

	int trainingNum;
	fin >> trainingNum;							//第4行是训练图的数量
	training_shapes_.resize(trainingNum);
	bounding_box_.resize(trainingNum);

	for (int i = 0; i < trainingNum; i++) {				//两行两行一组读入训练图的参数
		BoundingBox temp;
		fin >> temp.startX >> temp.startY >> temp.width >> temp.height >> temp.centerX >> temp.centerY;		//组合内：第一行是boundingBox的6个参数
		bounding_box_[i] = temp;

		Mat_<double> temp1(landmark_num_, 2);
		for (int j = 0; j < landmark_num_; j++) {				//组合内：第二行是29个坐标（共58个数），作为landmark参数
			fin >> temp1(j, 0) >> temp1(j, 1);
		}
		training_shapes_[i] = temp1;
	}				//一直读到了2694行

	fernCascades.resize(first_level_num_);
	for (int i = 0; i < first_level_num_; i++) {
		fernCascades[i].read(fin);
	}
}

Mat_<double> ShapeRegressor::predict(const Mat_<uchar>& image, const BoundingBox& bounding_box, int initial_num) {
	// generate multiple initializations
	Mat_<double> result = Mat::zeros(landmark_num_, 2, CV_64FC1);
	RNG random_generator(getTickCount());
	for (int i = 0; i < initial_num; i++) {
		random_generator = RNG(i);
		int index = random_generator.uniform(0, training_shapes_.size());		//uniform函数可以指定随机数的范围
		Mat_<double> currentShape = training_shapes_[index];
		BoundingBox currentBoundingBox = bounding_box_[index];
        currentShape = Utils::projectShape(currentShape, currentBoundingBox);		//projectShape函数将当前的shape等比例缩放到(0,1)的boundingBox内
        currentShape = Utils::reProjectShape(currentShape, bounding_box);				//reProjectShape函数将已经归一化的shape所放到真实的boundingBox内
		for (int j = 0; j < first_level_num_; j++) {
			Mat_<double> prediction = fernCascades[j].predict(image, bounding_box, meanShape, currentShape);
			// update current shape
            currentShape = prediction + Utils::projectShape(currentShape, bounding_box);
            currentShape = Utils::reProjectShape(currentShape, bounding_box);
		}
		result = result + currentShape;
	}
	return 1.0 / initial_num * result;
}

void ShapeRegressor::load(string path) {
	cout << "Loading model..." << endl;
	ifstream fin;
	fin.open(path.c_str());
	this->read(fin);
	fin.close();
	cout << "Model loaded successfully..." << endl;
}

