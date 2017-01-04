//
//  main.cpp
//  stereoVisionProject
//
//  Created by 楊恩慈 on 2016/11/10.
//  Copyright © 2016年 Piicksarn.Yang. All rights reserved.
//

#include "calibration.h"
#include <iostream>
#include <opencv2/highgui.hpp> // OpenCV window I/O
#include <opencv2/imgproc.hpp> // OpenCV image transformations
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/stitching.hpp>

void testDMap3(Mat img_1, Mat img_2) {
    Mat result, dspt1, dspt2, matchImg, output;
    BFMatcher matcher;
    vector<KeyPoint> surfkeyP1, surfkeyP2, keyp1, keyp2;
    vector<Point2f> obj;
    vector<Point2f> scene;
    vector<DMatch> matches;
    vector<DMatch> good_matches;
    
    /*
     
     ⌈‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾⌉
     | Using SURF, detect the keypoints first, and then compute the       |
     |    description, saving in the dsp1 and dsp2.                       |
     |____________________________________________________________________|
     
     */
    
    Ptr<xfeatures2d::SURF> surf = xfeatures2d::SURF::create();
    surf -> setHessianThreshold(50);
    surf -> detect(img_1, keyp1);
    surf -> detect(img_2, keyp2);
    surf -> compute(img_1, keyp1, dspt1);
    surf -> compute(img_2, keyp2, dspt2);
    
    /*
     
     ⌈‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾⌉
     | We need to select the good matching result to show.                |
     | If we didn't do the selection before using drawMatches function,   |
     |    the lines shown on the result img will too much.                |
     |____________________________________________________________________|
     
     */
    
    double max_dist = 0;
    double min_dist = 50;
    matcher.match( dspt1, dspt2,matches );
    for(int i = 0; i < dspt1.rows; i++) {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }
    for(int i = 0; i < dspt1.rows; i++)
        if(matches[i].distance < 3 * min_dist)
            good_matches.push_back(matches[i]);
    
    drawMatches(img_1, keyp1, img_2, keyp2,
                good_matches, matchImg, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imshow("Good_Matches", matchImg);
    
//    /*
//     
//     ⌈‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾⌉
//     | In the code shown below, the purpose is to stitch the images by    |
//     |    using the good matching result.                                 |
//     | First, to find the homography and the second is to wrap the        |
//     |    origin image to homography result.                              |
//     | Finally, we need to use copyTo function to stitch the results.     |
//     |____________________________________________________________________|
//     
//     */
//    
//    for(int i = 0; i < good_matches.size(); i++) {
//        obj.push_back( keyp1[ good_matches[i].queryIdx ].pt );
//        scene.push_back( keyp2[ good_matches[i].trainIdx ].pt );
//    }
//    
//    Mat H = findHomography( obj, scene, CV_RANSAC );
//    warpPerspective(img_2, output, H.inv(), Size(img_2.cols*2, img_2.rows));
//    Mat target_in_big_mat(output, Rect(0, 0, img_1.cols, img_1.rows));
//    img_1.copyTo(target_in_big_mat);
//    imshow("result", output);
}
void testDMap2(Mat left, Mat right) {
    Mat img1, img2, g1, g2;
    Mat disp, disp8;
    
    img1 = left.clone();
    img2 = right.clone();
    
    cvtColor(img1, g1, CV_BGR2GRAY);
    cvtColor(img2, g2, CV_BGR2GRAY);
    
//    StereoBM sbm(g1, g2, disp);
//    Ptr<StereoBM> sbm;
////    Ptr<StereoSGBM> sgbm = sgbm->createStereoBM();
////    sgbm->compute(g1,g2,disp);
//    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
//    sbm.state->SADWindowSize = 9;
//    sbm.state->numberOfDisparities = 112;
//    sbm.state->preFilterSize = 5;
//    sbm.state->preFilterCap = 61;
//    sbm.state->minDisparity = -39;
//    sbm.state->textureThreshold = 507;
//    sbm.state->uniquenessRatio = 0;
//    sbm.state->speckleWindowSize = 0;
//    sbm.state->speckleRange = 8;
//    sbm.state->disp12MaxDiff = 1;
    
    Ptr<StereoSGBM> sgbm = StereoSGBM::create(16*17, 21, 3);
    sgbm->compute(g1,g2,disp);

//    StereoSGBM sgbm;
//    sgbm.SADWindowSize = 5;
//    sgbm.numberOfDisparities = 192;
//    sgbm.preFilterCap = 4;
//    sgbm.minDisparity = -64;
//    sgbm.P2 = 2400;
//    
//    sgbm(g1, g2, disp);
        normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
    
    imshow("left", img1);
    imshow("right", img2);
    imshow("disp", disp8);
    
    
}
void testDMap(Mat left, Mat right) {
    cvtColor(left, left, CV_RGB2GRAY);
    cvtColor(right, right, CV_RGB2GRAY);
    Mat imgDisparity16S = Mat( left.rows, left.cols, CV_16S );
    Mat imgDisparity8U = Mat( left.rows, left.cols, CV_8UC1 );

//    if( left.empty() || right.empty() ) {
//        cout<< " --(!) Error reading images " << endl;
//        return;
//    }

    imshow("test", right);
    //-- 2. Call the constructor for StereoBM
    int ndisparities = 16*5;   /**< Range of disparity */
    int SADWindowSize = 13; /**< Size of the block window. Must be odd */

    Ptr<StereoBM> sbm = StereoBM::create( ndisparities, SADWindowSize );

    //-- 3. Calculate the disparity image
    sbm->compute( left, right, imgDisparity16S );

    //-- Check its extreme values
    double minVal; double maxVal;

    minMaxLoc( imgDisparity16S, &minVal, &maxVal );

    printf("Min disp: %f Max value: %f \n", minVal, maxVal);

    //-- 4. Display it as a CV_8UC1 image
    imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
//
    imshow( "windowDisparity", imgDisparity8U );

}

int main(){
    
    Mat left = imread("/Users/yangenci/Desktop/left.bmp", 1);
    Mat right = imread("/Users/yangenci/Desktop/right.bmp", 1);
    
    CameraCalibrator cameraCalibrator;
    cameraCalibrator.Initialize(left, right, Size(9,6));
//    testDMap3(myCameraCalibrator.getLeftResult(), myCameraCalibrator.getRightResult());
    waitKey();
    return 0;
}
