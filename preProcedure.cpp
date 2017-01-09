/*
 *  Created by Piicksarn.
 *  Copyright © 2017 Piicksarn.Yang. All rights reserved.
 */
#include "preProcedure.hpp"

void Preprocedure::Initialize(Size size){
    leftCamera.Initialize(Size(9,6), 1);
    rightCamera.Initialize(Size(9,6), 2);
}

void Preprocedure::calibrate(Mat left, Mat right) {
    leftCamera.calibrate(left);
    rightCamera.calibrate(right);
    leftImage = leftCamera.getResultImg();
    rightImage = rightCamera.getResultImg();
    imageRectification();

}
void Preprocedure::imageRectification() {
    imgSize = leftCamera.getImgSize();
    testStereoCalibrate();
    testStereoRectify();

}
void Preprocedure::testStereoRectify() {
    Mat Q;
    Rect validRoi[2];
    stereoRectify(leftCamera.getCameraMat(), leftCamera.getDistory(),
                  rightCamera.getCameraMat(), rightCamera.getDistory(),
                  imgSize, rvecOfTwo, tvecOfTwo, R1, R2, P1, P2,
                  Q, CALIB_ZERO_DISPARITY, 0,imgSize, &validRoi[0], &validRoi[1]);
    Mat map1, map2, test, test2, test3;
    Mat img = imread("/Users/yangenci/Desktop/1/1-16.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    initUndistortRectifyMap(leftCamera.getCameraMat(), leftCamera.getDistory(), R1, P1, imgSize, CV_16SC2, map1, map2);
    remap(img, test, map1, map2, INTER_LINEAR);
    cvtColor(test, test, COLOR_GRAY2BGR);

    Mat img2 = imread("/Users/yangenci/Desktop/2/2-16.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    initUndistortRectifyMap(rightCamera.getCameraMat(), rightCamera.getDistory(), R2, P2, imgSize, CV_32F, map1, map2);
    remap(img2, test2, map1, map2, INTER_LINEAR);
    cvtColor(test2, test2, COLOR_GRAY2BGR);
    
    hconcat(test, test2, test3);
        for (int j = 0; j <= test3.rows; j += 16)
            line(test3, Point(0, j), Point(test3.cols, j), Scalar(0, 255, 0), 1, 8);

    imshow("test-right", test3);

}
void Preprocedure::testStereoCalibrate() {
    Mat R, T, E, F;
    double rms = stereoCalibrate(leftCamera.getObjectPoint(), leftCamera.getImgPoint(), rightCamera.getImgPoint(),
                                 leftCamera.getCameraMat(), leftCamera.getDistory(),
                                 rightCamera.getCameraMat(), rightCamera.getDistory(),
                                 imgSize, R, T, E, F, CV_CALIB_FIX_INTRINSIC,TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 1e-6));
    cout << "done with RMS error=" << rms << endl;
    cout<<"F in stereoCalibrate: "<<F;

    rvecOfTwo = R;
    tvecOfTwo = T;


}
// Mat Preprocedure::getDisparityMap() {
//
//
// }
