/*
 *  Created by Piicksarn.
 *  Copyright © 2017 Piicksarn.Yang. All rights reserved.
 */

#include "depthCreation.hpp"
using namespace cv::ximgproc;
void Disparity::initialize(Mat left, Mat right) {
    left_img = left;
    right_img = right;
    //     left = imread("/Users/yangenci/Desktop/Data/left.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
    //     right = imread("/Users/yangenci/Desktop/Data/right.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
    compute_disp();
    optimization();
}

void Disparity::compute_disp() {
    cvtColor(left_img, left_img, CV_BGR2GRAY, 1);
    cvtColor(right_img, right_img, CV_BGR2GRAY, 1);
    filtering_rough();
    filtering_detail();
}

void Disparity::filtering_rough() {
    Mat left, right;
    Mat left_disp,right_disp;
    Mat filtered_disp;
    Ptr<DisparityWLSFilter> wls_filter;
    int max_disp = 16 * 5;
    int wsize = 5;
    double vis_mult = 1.0;
    double lambda = 9000;
    double sigma = 1.5;
    
    left = left_img.clone();
    right = right_img.clone();
    
    Ptr<StereoBM> left_matcher = StereoBM::create(max_disp, wsize);
    Ptr<StereoMatcher> right_matcher = createRightMatcher(left_matcher);
    
    wls_filter = createDisparityWLSFilter(left_matcher);
    
    left_matcher-> compute(left, right, left_disp);
    right_matcher-> compute(right, left, right_disp);
    
    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma);
    wls_filter->filter(left_disp, left_img, filtered_disp, right_disp);

    getDisparityVis(filtered_disp, rough_result, vis_mult);
    imshow("filtered disparity", rough_result);
}
void Disparity::filtering_detail() {
    Mat left, right;
    Mat left_disp, right_disp;
    Mat filtered_disp;
    Ptr<DisparityWLSFilter> wls_filter;
    int max_disp = 16 * 5;
    int wsize = 5;
    double vis_mult = 1.0;
    double lambda = 9000;
    double sigma = 1.5;
    
    left = left_img.clone();
    right = right_img.clone();
    
    Ptr<StereoBM> matcher  = StereoBM::create(max_disp, wsize);
    matcher-> setTextureThreshold(10);
    matcher-> setUniquenessRatio(0);
    
    wls_filter = createDisparityWLSFilterGeneric(false);
    wls_filter-> setDepthDiscontinuityRadius((int)ceil(0.33 * wsize));
    
    matcher-> compute(left, right, left_disp);
    wls_filter-> setLambda(lambda);
    wls_filter-> setSigmaColor(sigma);
    wls_filter-> filter(left_disp, left_img, filtered_disp, Mat(), Rect());
    
    getDisparityVis(filtered_disp, detail_result, vis_mult);
    imshow("2", detail_result);
}

void Disparity::optimization() {
}