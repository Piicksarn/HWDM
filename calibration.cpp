//
//  calibration.cpp
//  stereoVisionProject
//
//  Created by 楊恩慈 on 2016/11/10.
//  Copyright © 2016年 Piicksarn.Yang. All rights reserved.
//

#include "calibration.h"

void CameraCalibrator::Initialize(Mat left, Mat right, Size size){
    leftImage = left;
    rightImage = right;
    setFileList();
    setBorderSize(size);
    addChessboardPoints();
    calibrate(left, right);
    imshow("left-result", leftImage);
    imshow("right-result", rightImage);
}
void CameraCalibrator::setFileList(){
    m_l_fileList.clear();
    m_r_fileList.clear();

    for (int i = 1; i <= 2; i++) {
        String imgFolder = to_string(i) + "/" + to_string(i) + "-";
        for (int j = 1; j <= 20; j++) {
            String filename = "/Users/yangenci/Desktop/";
            filename = filename + imgFolder + to_string(j) + ".bmp";
            cout<<filename<<endl;
            if(i == 1)
                m_l_fileList.push_back(filename);
            else
                m_r_fileList.push_back(filename);
        }
    }
}

void CameraCalibrator::setBorderSize(const Size &borderSize){
    m_borderSize = borderSize;
}

void CameraCalibrator::addChessboardPoints(){
    vector<Point2f> l_srcCandidateCorners;
    vector<Point3f> l_dstCandidateCorners;

    vector<Point2f> r_srcCandidateCorners;
    vector<Point3f> r_dstCandidateCorners;

    // Initialize the dist matrix
    for(int i=0; i<m_borderSize.height; i++){
        for(int j=0; j<m_borderSize.width; j++){
            l_dstCandidateCorners.push_back(Point3f(i, j, 0.0f));
            r_dstCandidateCorners.push_back(Point3f(i, j, 0.0f));
        }
    }
    cout<<"done"<<endl;

    for (int i = 0; i < m_l_fileList.size(); i++) {
        //Read all the images for collibration and set it to gray scale
        Mat image_left = imread(m_l_fileList[i],CV_LOAD_IMAGE_GRAYSCALE);
        Mat image_right = imread(m_r_fileList[i],CV_LOAD_IMAGE_GRAYSCALE);


        // @findChessboardCorners
        //    Return boolean type.
        //    The opencv function:(input image, chess board size, OutputArray corners, flags(option)).
        findChessboardCorners(image_left, m_borderSize, l_srcCandidateCorners);
        findChessboardCorners(image_right, m_borderSize, r_srcCandidateCorners);

        // @TermCriteria (Type, maxCount, epsilon)
        //    Type: The type of termination criteria
        //    maxCount: The maximum number of iterations or elements to compute.
        //    epsilon: The desired accuracy or change in parameters at which the iterative algorithm stops.
        TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);

        // @cornerSubPix (Input Image, Corners, Window Size, ZeroZone, Criteria)
        //    To search the corner of the image, and set the end condition by using TermCriteria result.
        cornerSubPix(image_left, l_srcCandidateCorners, Size(5,5), Size(-1,-1), param);
        cornerSubPix(image_right, r_srcCandidateCorners, Size(5,5), Size(-1,-1), param);

        // If the corner detection result count is match to the count that we input(by borderSize),
        // We then consider it as the collibration point
        if(l_srcCandidateCorners.size() == m_borderSize.area()){
            m_l_srcPoints.push_back(l_srcCandidateCorners);
            m_l_dstPoints.push_back(l_dstCandidateCorners);
        }
        if(r_srcCandidateCorners.size() == m_borderSize.area()){
            m_r_srcPoints.push_back(r_srcCandidateCorners);
            m_r_dstPoints.push_back(r_dstCandidateCorners);
        }
    }
}

void CameraCalibrator::addPoints(vector<vector<Point2f>> src, vector<vector<Point3f>> dst, const vector<Point2f> &srcCorners, const vector<Point3f> &dstCorners){
    src.push_back(srcCorners);
    dst.push_back(dstCorners);
}

void CameraCalibrator::calibrate(Mat left, Mat right){

    Size leftImgSize = left.size();
    Size rightImgSize = right.size();

    Mat leftCamMat, leftDistCoeffs, left_map1, left_map2;
    Mat rightCamMat, rightDistCoeffs, right_map1, right_map2;

    vector<Mat> rvecs, tvecs, right_rvecs, right_tvecs, left_rvecs, left_tvecs;

    // Left Camera Calibrate
    calibrateCamera(m_l_dstPoints, m_l_srcPoints, leftImgSize, leftCamMat, leftDistCoeffs, left_rvecs, left_tvecs);
    initUndistortRectifyMap(leftCamMat, leftDistCoeffs, Mat(), Mat(), leftImgSize, CV_32F, left_map1, left_map2);
    remap(left, leftImage, left_map1, left_map2, INTER_LINEAR);

    // Right Camera Calibrate
    calibrateCamera(m_r_dstPoints, m_r_srcPoints, rightImgSize, rightCamMat, rightDistCoeffs, right_rvecs, right_tvecs);
    initUndistortRectifyMap(rightCamMat, rightDistCoeffs, Mat(), Mat(), rightImgSize, CV_32F, right_map1, right_map2);
    remap(right, rightImage, right_map1, right_map2, INTER_LINEAR);



    // cout<<"Camera Matrix : "<<cameraMatrix<<endl;
    // cout<<"Distroy Coefficient : "<<distCoeffs<<endl;
    // cout<<"Rotation Vector : "<<rvecs[0]<<endl;
    // cout<<"Traslation Vector : "<<tvecs[0]<<endl;
}
Mat CameraCalibrator::getLeftResult() {
    return leftImage;
}
Mat CameraCalibrator::getRightResult() {
    return rightImage;
}
