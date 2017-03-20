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
}

void Disparity::compute_disp() {
    cvtColor(left_img, left_img, CV_BGR2GRAY, 1);
    cvtColor(right_img, right_img, CV_BGR2GRAY, 1);
    filtering_detail();
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
    
//    Ptr<StereoBM> matcher  = StereoBM::create(max_disp, wsize);
//    matcher-> setTextureThreshold(10);
//    matcher-> setUniquenessRatio(0);
//
//    wls_filter = createDisparityWLSFilterGeneric(false);
//    wls_filter-> setDepthDiscontinuityRadius((int)ceil(0.33 * wsize));
//    
//    matcher-> compute(left, right, left_disp);
    Ptr<StereoSGBM> matcher  = StereoSGBM::create(0,max_disp,wsize);
    matcher->setUniquenessRatio(0);
    matcher->setDisp12MaxDiff(1000000);
    matcher->setSpeckleWindowSize(0);
    matcher->setP1(24*wsize*wsize);
    matcher->setP2(96*wsize*wsize);
    matcher->setMode(StereoSGBM::MODE_SGBM_3WAY);
    wls_filter = createDisparityWLSFilterGeneric(false);
    wls_filter->setDepthDiscontinuityRadius((int)ceil(0.5*wsize));
    
    matcher->compute(left,right,left_disp);

    wls_filter-> setLambda(lambda);
    wls_filter-> setSigmaColor(sigma);
    wls_filter-> filter(left_disp, left_img, filtered_disp, Mat(), Rect());

    getDisparityVis(filtered_disp, detail_result, vis_mult);
    double minVal; double maxVal;
    minMaxLoc(detail_result, &minVal, &maxVal );
    detail_result.convertTo( detail_result, CV_8UC1, 255/(maxVal - minVal));
}

Mat Disparity::get_result() {
    return detail_result;
}
