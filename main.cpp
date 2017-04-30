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
#include "depthCreation.hpp"
#include "Roicapture.hpp"

int main(){

    Preprocedure preprocedure;
    Disparity disparity;
    Capture capture;
    Mat leftFrame, rightFrame, result, frame;
    int count = 0;

    VideoCapture cap("/Users/yangenci/Desktop/Data/1-8m-2.mp4");

    if (!cap.isOpened()) {
        cout << "Cannot open the video file." << endl;
        return -1;
    }
    while(1) {
        cap.read(frame);
        if(!frame.empty()) {

            Mat left = frame(Rect(frame.cols / 4, 0, frame.cols/2, frame.rows / 2));
            Mat right = frame(Rect(frame.cols / 4, frame.rows / 2, frame.cols/2, frame.rows / 2));

            // 1. Do preprocedure: camera calibration, image retrification
            preprocedure.Initialize(Size(9, 6), left, right);
            leftFrame = preprocedure.getLeft();
            rightFrame = preprocedure.getRight();

            // 2. Calculate the disparity map
            disparity.initialize(leftFrame,  rightFrame);
            // 3. Determine and Capture the roi(hand part) of image
            if(count == 0)
                capture.initialize(disparity.get_result(), leftFrame);
            else {
                capture.update_frame(leftFrame, disparity.get_result());
                if(count > 2 && !capture.target_exit()) {
                    capture.cal_roi();
                }
                else if(capture.target_exit())
                    capture.do_tracking();
            }

            // 4. Show the results
            //            hconcat(leftFrame, rightFrame, result);
            //            imshow("stereo", disparity.get_result());
            //            imshow("result", result);
            //

            for (int j = 0; j <= result.rows; j += 12)
                line(result, Point(0, j), Point(result.cols, j), Scalar(0, 255, 0), 1, 8);

            putText(result, to_string(count), Point(100,100), 0, 1, Scalar(0,0,255), 7);
            // imshow("test-right", result);
            count ++;
        }
        else
            break;
        waitKey();

    }
    return 0;
}
