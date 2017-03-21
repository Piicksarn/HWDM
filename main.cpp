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

    // Do preprocedure
    Preprocedure preprocedure;
    Disparity disparity;
    Mat leftFrame, rightFrame, result, frame;
    VideoCapture cap("/Users/yangenci/Desktop/Data/piicksarn2.mp4");

    if (!cap.isOpened()) {
        cout << "Cannot open the video file." << endl;
        return -1;
    }
    int count = 0;
    Capture capture;
    while(1) {
        cap.read(frame);
        if(!frame.empty()) {

            Mat left = frame(Rect(frame.cols / 4, 0, frame.cols/2, frame.rows / 2));
            Mat right = frame(Rect(frame.cols / 4, frame.rows / 2, frame.cols/2, frame.rows / 2));

            preprocedure.Initialize(Size(9, 6), left, right);
            leftFrame = preprocedure.getLeft();
            rightFrame = preprocedure.getRight();

            disparity.initialize(leftFrame,  rightFrame);
            if(count == 0)
                capture.initialize(disparity.get_result(), leftFrame);
            else
                capture.update_frame(leftFrame, disparity.get_result());
            if(count > 1)
                capture.cal_roi();

            hconcat(leftFrame, rightFrame, result);
            for (int j = 0; j <= result.rows; j += 12)
                line(result, Point(0, j), Point(result.cols, j), Scalar(0, 255, 0), 1, 8);

            putText(result, to_string(count), Point(100,100), 0, 1, Scalar(0,0,255), 7);
            // imshow("test-right", result);
            imshow("result", disparity.get_result());
            count ++;
        }
        else
            break;
        waitKey();

    }
    return 0;
}
