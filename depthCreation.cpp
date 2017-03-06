/*
 *  Created by Piicksarn.
 *  Copyright © 2017 Piicksarn.Yang. All rights reserved.
 */

#include "depthCreation.hpp"

void Disparity::initialize(Mat left, Mat right) {
  left_img = left;
  right_img = right;
  //     left = imread("/Users/yangenci/Desktop/Data/left.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
  //     right = imread("/Users/yangenci/Desktop/Data/right.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
  compute_disp();
}

void Disparity::compute_disp() {
     Mat imgDis16S = Mat( left_img.rows, left_img.cols, CV_16S  );
       Mat imgDisparity8U = Mat( left_img.rows, left_img.cols, CV_8UC1 );
       int disNum = 16 * 5;
       int size = 9;
       Ptr<StereoBM> sbm = StereoBM::create(disNum, size);
       sbm->setPreFilterCap(31);
       sbm->setMinDisparity(0);
       sbm->setNumDisparities(80);
       sbm->setTextureThreshold(10);
       sbm->setUniquenessRatio(5); // original setting is 15
       sbm->setSpeckleRange(32);
       sbm->setDisp12MaxDiff(1);
       cvtColor(left_img, left_img, CV_BGR2GRAY, 1);
       cvtColor(right_img, right_img, CV_BGR2GRAY, 1);

       sbm->compute(left_img, right_img, imgDis16S);
       double minVal; double maxVal;
       minMaxLoc(imgDis16S, &minVal, &maxVal );
       printf("Min disp: %f Max value: %f \n", minVal, maxVal);
       //-- 4. Display it as a CV_8UC1 image
       imgDis16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
       imshow( "windowDisparity", imgDisparity8U );
       imshow("result", imgDis16S);
//       return imgDis16S;

}
void Disparity::optimization() {



}
