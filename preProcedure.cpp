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

    composeRT(leftCamera.getRvec(), leftCamera.getTvec(), rightCamera.getRvec(), rightCamera.getTvec(), rvecOfTwo, tvecOfTwo);
    imageRectification();
    //
    //  cout<<"rvec of two: "<<rvecOfTwo;
    //  cout<<"tvec of two: "<<tvecOfTwo;
//    imshow("left", leftImage);
//    imshow("right", rightImage);
}
void Preprocedure::imageRectification() {
    imgSize = leftCamera.getImgSize();

    cout<<"==========Both============"<<endl;
         cout<<"R of two: "<<rvecOfTwo<<endl;
         cout<<"T of Two: "<<tvecOfTwo<<endl;
    Mat map1, map2, test;

    testStereoCalibrate();
    testStereoRectify();
//    testFundamental();
    //  cout<<"==========Left============"<<endl;
    //  cout<<"Left Camera: "<<endl;
    //  cout<<"Rotation: "<<R1<<endl;
    //  cout<<"Traslation: "<<P1<<endl;
    //  cout<<"==========Right============"<<endl;
    //  cout<<"Right Camera: "<<endl;
    //  cout<<"Rotation: "<<R2<<endl;
    //  cout<<"Traslation: "<<P2<<endl;
    //  cout<<"======================"<<endl;

  //  initUndistortRectifyMap(leftCamera.getCameraMat(), leftCamera.getDistory(), Mat(), Mat(), imgSize, CV_32F, map1, map2);
    
    
    Mat Q;
    Rect validRoi[2];
    stereoRectify(leftCamera.getCameraMat(), leftCamera.getDistory(),
                  rightCamera.getCameraMat(), rightCamera.getDistory(),
                  imgSize, rvecOfTwo, tvecOfTwo, R1, R2, P1, P2, Q,
                  CALIB_ZERO_DISPARITY, 1, imgSize, &validRoi[0], &validRoi[1]);
    
    
    Mat img = imread("/Users/yangenci/Desktop/1/1-16.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    initUndistortRectifyMap(leftCamera.getCameraMat(), leftCamera.getDistory(), Mat(), Mat(), imgSize, CV_32F, map1, map2);
    remap(img, test, map1, map2, INTER_LINEAR);
    cvtColor(test, test, COLOR_GRAY2BGR);
    imshow("test-left", test);

    Mat img2 = imread("/Users/yangenci/Desktop/2/2-16.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    initUndistortRectifyMap(rightCamera.getCameraMat(), rightCamera.getDistory(), Mat(), Mat(), imgSize, CV_32F, map1, map2);
    remap(img2, test, map1, map2, INTER_LINEAR);
    cvtColor(test, test, COLOR_GRAY2BGR);
    imshow("test- right", test);
    
    
    
    
    namedWindow("canvas", 1);
    cout << "read the picture for 3d-reconstruction...";
    Mat canvas(imgSize.height, imgSize.width * 2, CV_8UC3), viewLeft, viewRight;
    Mat canLeft = canvas(Rect(0, 0, imgSize.width, imgSize.height));
    Mat canRight = canvas(Rect(imgSize.width, 0, imgSize.width, imgSize.height));
    viewLeft = img;
    viewRight = img2;
    viewLeft.copyTo(canLeft);
    viewRight.copyTo(canRight);
    cout << "done" << endl;
    
    
    Mat rmapFirst[2], rmapSec[2], rviewFirst, rviewSec;
    initUndistortRectifyMap(leftCamera.getCameraMat(), leftCamera.getDistory(), R1, P1,imgSize, CV_16SC2, rmapFirst[0], rmapFirst[1]);
    initUndistortRectifyMap(rightCamera.getCameraMat(), rightCamera.getDistory(), R2, P2,imgSize, CV_16SC2, rmapSec[0], rmapSec[1]);
    remap(viewLeft, rviewFirst, rmapFirst[0], rmapFirst[1], INTER_LINEAR);
    remap(viewRight, rviewSec, rmapSec[0], rmapSec[1], INTER_LINEAR);
    rviewFirst.copyTo(canLeft);
    rviewSec.copyTo(canRight);
    rectangle(canLeft, validRoi[0], Scalar(255, 0, 0), 3, 8);
    rectangle(canRight, validRoi[1], Scalar(255, 0, 0), 3, 8);
    for (int j = 0; j <= canvas.rows; j += 16)
        line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
    cout << "stereo rectify done" << endl;
    imshow("canvas", canvas);
    
    
    
    
    
    
    
    
    
    
}
void Preprocedure::testStereoRectify() {
//  Mat Q;
//  Rect validRoi[2];
//  stereoRectify(leftCamera.getCameraMat(), leftCamera.getDistory(),
//                 rightCamera.getCameraMat(), rightCamera.getDistory(),
//                 imgSize, rvecOfTwo, tvecOfTwo, R1, R2, P1, P2, Q,
//                 CALIB_ZERO_DISPARITY, 1, imgSize, &validRoi[0], &validRoi[1]);

}
void Preprocedure::testStereoCalibrate() {
  Mat R, T, E, F;
  // Test for the position of the imagePoint of left Camera
  // Mat img = leftCamera.getResultImg().clone();
//  vector<Point2f> imgPoints = leftCamera.getImgPoint();
//  for (int i = 0; i < imgPoints.size(); i++) {
//      circle(img, imgPoints[i], 10, Scalar(255,255,255),CV_FILLED, 8,0);
//  }
//  for (int i = 0; i < imgPoints.size(); i++) {
//      putText(img, to_string(i), imgPoints[i], FONT_HERSHEY_COMPLEX, 0.3, Scalar(0,0,0));
//  }
//  imshow("left", img);
//  // Test for the position of the imagePoint of right Camera
//  Mat img2 = rightCamera.getResultImg().clone();
//  vector<Point2f> img2Points = rightCamera.getImgPoint();
//  for (int i = 0; i < img2Points.size(); i++) {
//      circle(img2, img2Points[i], 10, Scalar(255,255,255),CV_FILLED, 8,0);
//  }
//  for (int i = 0; i < img2Points.size(); i++) {
//      putText(img2, to_string(i), img2Points[i], FONT_HERSHEY_COMPLEX, 0.3, Scalar(0,0,0));
//  }
//  imshow("right", img2);

  stereoCalibrate(leftCamera.getObjectPoint(), leftCamera.getImgPoint(), rightCamera.getImgPoint(),
                  leftCamera.getCameraMat(), leftCamera.getDistory(),
                  rightCamera.getCameraMat(), rightCamera.getDistory(),
                  imgSize, R, T, E, F);
rvecOfTwo = R;
tvecOfTwo = T;
cout<<"left cam: "<<leftCamera.getCameraMat()<<endl;
cout<<"left dist: "<<leftCamera.getDistory()<<endl;
cout<<"right cam: "<<rightCamera.getCameraMat()<<endl;
cout<<"right dist: "<<rightCamera.getDistory()<<endl;
cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
 cout<<"Rotation: "<<R<<endl;
 cout<<"Traslation: "<<T<<endl;
// leftCamera.calibrateAll();
// rightCamera.calibrateAll();


}
void Preprocedure::testFundamental() {
     Mat F;
     F = findFundamentalMat(leftCamera.getImgPoint(), rightCamera.getImgPoint(), FM_RANSAC, 3, 0.99);
     Mat H1, H2;
     stereoRectifyUncalibrated(leftCamera.getImgPoint(), rightCamera.getImgPoint(), F, imgSize, H1, H2, 3);

     R1 = leftCamera.getCameraMat().inv()*H1*leftCamera.getCameraMat().inv();
     R2 = rightCamera.getCameraMat().inv()*H1*rightCamera.getCameraMat().inv();
     P1 = leftCamera.getCameraMat();
     P2 = rightCamera.getCameraMat();

}
// Mat Preprocedure::getDisparityMap() {
//
//
// }
