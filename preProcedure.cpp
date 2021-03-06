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
    /*
     * Do stereo rectify.
     * As the result of rectify is so bad, use unrectify result instead.
     * The rectify method should be modify in the future.
     */
    
    Mat map1, map2, Q;
    Rect validRoi[2];
    
    stereoRectify(leftCam, leftDis, rightCam, rightDis, imgSize, rvecOfTwo, tvecOfTwo, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 0.0, imgSize, &validRoi[0], &validRoi[1]);
    
    //initUndistortRectifyMap(leftCam, leftDis, R1, P1, imgSize, CV_16SC2, map1, map2);
    initUndistortRectifyMap(leftCam, leftDis, Mat(), Mat(), imgSize, CV_16SC2, map1, map2);
    remap(left, leftResult, map1, map2, INTER_LINEAR);
    
    //initUndistortRectifyMap(rightCam, rightDis, R2, P2, imgSize, CV_32F, map1, map2);
    initUndistortRectifyMap(rightCam, rightDis, Mat(), Mat(), imgSize, CV_32F, map1, map2);
    remap(right, rightResult, map1, map2, INTER_LINEAR);
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
Mat Preprocedure::getLeft() {
    return leftResult;
}
Mat Preprocedure::getRight() {
    return rightResult;
}
