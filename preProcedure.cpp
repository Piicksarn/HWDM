/*
 *  Created by Piicksarn.
 *  Copyright © 2017 Piicksarn.Yang. All rights reserved.
 */
#include "preProcedure.hpp"

void Preprocedure::Initialize(Size size, Mat left, Mat right){
    FileStorage fs("/Users/yangenci/git/HWDM/paraData.yml", FileStorage::READ);
    FileNode fileStatus = fs["status"];
    status = fileStatus.empty();
    fs.release();
    if (status == 1) {
        leftCamera.Initialize(Size(9,6), 1);
        rightCamera.Initialize(Size(9,6), 2);
        CameraCalibrate();
        imgSize = leftCamera.getImgSize();
        testStereoCalibrate();
    }
    else
        readPareFile();
    testStereoRectify(left, right);

}

void Preprocedure::CameraCalibrate() {
      Mat left = imread("/Users/yangenci/Desktop/Data/left4/56.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
      Mat right = imread("/Users/yangenci/Desktop/Data/right4/56.jpeg", CV_LOAD_IMAGE_GRAYSCALE);

      leftCamera.calibrate(left);
      rightCamera.calibrate(right);
}

void Preprocedure::testStereoRectify(Mat left, Mat right) {

    Mat map1, map2, test, test2, test3;
    Mat img = left;
    Mat Q;
    Rect validRoi[2];
    stereoRectify(leftCam, leftDis, rightCam, rightDis, imgSize, rvecOfTwo, tvecOfTwo, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 0.0, imgSize, &validRoi[0], &validRoi[1]);

//    initUndistortRectifyMap(leftCam, leftDis, R1, P1, imgSize, CV_16SC2, map1, map2);
    initUndistortRectifyMap(leftCam, leftDis, Mat(), Mat(), imgSize, CV_16SC2, map1, map2);

    remap(img, test, map1, map2, INTER_LINEAR);
//    cvtColor(test, test, COLOR_GRAY2BGR);

    Mat img2 = right;
//    initUndistortRectifyMap(rightCam, rightDis, R2, P2, imgSize, CV_32F, map1, map2);
    initUndistortRectifyMap(rightCam, rightDis, Mat(), Mat(), imgSize, CV_32F, map1, map2);

    remap(img2, test2, map1, map2, INTER_LINEAR);
//    cvtColor(test2, test2, COLOR_GRAY2BGR);

    hconcat(test, test2, test3);
        for (int j = 0; j <= test3.rows; j += 12)
            line(test3, Point(0, j), Point(test3.cols, j), Scalar(0, 255, 0), 1, 8);

    imshow("test-right", test3);
    getDisparityMap(test, test2);
}
void Preprocedure::testStereoCalibrate() {
    Mat R, T, E, F;

    double rms = stereoCalibrate(leftCamera.getObjectPoint(), leftCamera.getImgPoint(), rightCamera.getImgPoint(), leftCamera.getCameraMat(), leftCamera.getDistory(), rightCamera.getCameraMat(), rightCamera.getDistory(), imgSize, R, T, E, F, CV_CALIB_FIX_INTRINSIC,TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 1e-6));
    cout<<endl;
    cout<<"rms: "<<rms<<endl;
    rvecOfTwo = R;
    tvecOfTwo = T;
    leftCam = leftCamera.getCameraMat();
    leftDis = leftCamera.getDistory();
    rightCam = rightCamera.getCameraMat();
    rightDis = rightCamera.getDistory();
    writeParaFile();
}
 Mat Preprocedure::getDisparityMap(Mat left, Mat right) {

//     left = imread("/Users/yangenci/Desktop/Data/left.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
//     right = imread("/Users/yangenci/Desktop/Data/right.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
     Mat imgDis16S = Mat( left.rows, left.cols, CV_16S  );
     Mat imgDisparity8U = Mat( left.rows, left.cols, CV_8UC1 );
     int disNum = 16 * 5;
     int size = 9;
     Mat test;
     Ptr<StereoBM> sbm = StereoBM::create(disNum, size);
     sbm->setPreFilterCap(31);
     sbm->setMinDisparity(0);
     sbm->setNumDisparities(80);
     sbm->setTextureThreshold(10);
     sbm->setUniquenessRatio(5); // original setting is 15
     sbm->setSpeckleRange(32);
     sbm->setDisp12MaxDiff(1);
     cvtColor(left, left, CV_BGR2GRAY, 1);
     cvtColor(right, right, CV_BGR2GRAY, 1);

     sbm->compute(left, right, imgDis16S);
     double minVal; double maxVal;
     minMaxLoc(imgDis16S, &minVal, &maxVal );
     printf("Min disp: %f Max value: %f \n", minVal, maxVal);
     //-- 4. Display it as a CV_8UC1 image
     imgDis16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
     imshow( "windowDisparity", imgDisparity8U );
//
//
//
     imshow("result", imgDis16S);
     return imgDis16S;
 }
 void Preprocedure::writeParaFile() {
    FileStorage fs("/Users/yangenci/git/HWDM/paraData.yml", FileStorage::WRITE);
    fs << "left Camera Mat" << leftCamera.getCameraMat();
    fs << "left Dis Mat" << leftCamera.getDistory();
    fs << "right Camera Mat" << rightCamera.getCameraMat();
    fs << "right Dis Mat" << rightCamera.getDistory();
    fs << "image Size" << imgSize;
    fs << "rvec of two" << rvecOfTwo;
    fs << "tvec of two" << tvecOfTwo;
    fs << "R1" << R1;
    fs << "R2" << R2;
    fs << "P1" << P1;
    fs << "P2" << P2;
    fs << "status" << 0;
    fs.release();
 }
void Preprocedure::readPareFile() {
  FileStorage fs("/Users/yangenci/git/HWDM/paraData.yml", FileStorage::READ);
  fs["left Camera Mat"] >> leftCam;
  fs["left Dis Mat"] >> leftDis;
  fs["image Size"] >> imgSize;
  fs["right Camera Mat"] >> rightCam;
  fs["right Dis Mat"] >> rightDis;
  fs["rvec of two"] >> rvecOfTwo;
  fs["tvec of two"] >> tvecOfTwo;
  fs["R1"] >> R1;
  fs["R2"] >> R2;
  fs["P1"] >> P1;
  fs["P2"] >> P2;
  fs.release();
}
