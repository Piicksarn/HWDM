//
//  calibration.h
//  stereoVisionProject
//
//  Created by 楊恩慈 on 2016/11/10.
//  Copyright © 2016年 Piicksarn.Yang. All rights reserved.
//

#include <cstdio>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class CameraCalibrator{
private:
    Size m_borderSize;
    Mat rightImage;
    Mat leftImage;
    Mat cameraMat;
    Mat rotationMat;
    Mat transMat;
    Mat distoryMat;
    
    vector<string> fileList;
    vector<vector<Point2f> > srcPointsMat;
    vector<vector<Point3f> > dstPointsMat;
    
    vector<vector<Point2f> > m_l_srcPoints;
    vector<vector<Point3f> > m_l_dstPoints;
    vector<vector<Point2f> > m_r_srcPoints;
    vector<vector<Point3f> > m_r_dstPoints;
    vector<string> m_l_fileList;
    vector<string> m_r_fileList;
public:
    void setFileList();
    void Initialize(Mat left, Mat right, Size size);
    void setBorderSize(const Size &borderSize);
    void addChessboardPoints();
    void addPoints(vector<vector<Point2f>> src, vector<vector<Point3f>> dst, const vector<Point2f> &imageCorners, const vector<Point3f> &objectCorners);
    void calibrate(Mat left, Mat right);
    Mat getRightResult();
    Mat getLeftResult();
};
