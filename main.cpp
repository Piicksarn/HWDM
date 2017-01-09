/*
 *  Created by Piicksarn.
 *  Copyright © 2017 Piicksarn.Yang. All rights reserved.
 */


#include <iostream>
#include <opencv2/highgui.hpp> // OpenCV window I/O
#include <opencv2/imgproc.hpp> // OpenCV image transformations
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/stitching.hpp>
#include "preProcedure.hpp"


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

int main(){

    Mat left = imread("/Users/yangenci/Desktop/1-19.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    Mat right = imread("/Users/yangenci/Desktop/2-19.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    // Do preprocedure
    Preprocedure preprocedure;
    preprocedure.Initialize(Size(9, 6));
    preprocedure.calibrate(left, right);


    waitKey();
    return 0;
}
