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
// #include "Roicapture.hpp"
#include "headfile/Track.hpp"
#include "headfile/FingerAnalysis.hpp"

// #include "headfile/Track.hpp"
void skinExtract(Mat frameInRGB, Mat mask) {
    Mat YCbCr;
    vector<Mat> planes;
    Mat frame = frameInRGB;
    cvtColor(frame, YCbCr, CV_RGB2YCrCb);
    split(YCbCr, planes);
    int cr_total = 0;
    int cr_total2 = 0;

    int cb_total = 0;
    int amount = 0;
    //運用迭代器訪問矩陣元素
    MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),
                        it_Cb_end = planes[1].end<uchar>();
    MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
//    MatIterator_<uchar> it_skin = skinArea.begin<uchar>();
    //人的皮膚顏色在YCbCr色度空間的分佈範圍:100<=Cb<=127, 138<=Cr<=170
    for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, amount++)
    {
        cr_total += *it_Cr;
        cb_total += *it_Cb;
//        cr_total2 += planes;
        // if (138 <= *it_Cr &&  *it_Cr <= 170 && 100 <= *it_Cb &&  *it_Cb <= 127)
        //     *it_skin = 255;
        // else
        //     *it_skin = 0;
    }


    // Y: 0 ~255
    // Cr: 133 ~ 177
    // Cb: 98 ~ 122

    // cout<<"Cr average: "<<cr_total/amount<<endl;
    // cout<<"Cb average: "<<cb_total/amount<<endl;
    // cout<<"---------------------------------------"<<endl;

//    inRange(YCbCr, Scalar(0, cb_total/amount - 5, cr_total/amount - 5), Scalar(255, cb_total/amount + 5, cr_total/amount + 5), YCbCr);
    // inRange(YCbCr, Scalar(0, 77, 133), Scalar(255, 127, 173), YCbCr);
    // imshow("ycbcr", YCbCr);
    // imshow("frame", frame);

    // //膨脹和腐蝕，膨脹可以填補凹洞（將裂縫橋接），腐蝕可以消除細的凸起（「斑點」噪聲）
    // dilate(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));
    // erode(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));


}
int main(){
    Preprocedure preprocedure;
    Disparity disparity;
    Capture capture;
    Track track;
    Analysis analysis;
    Mat leftFrame, rightFrame, result, frame;
    int count = 0;

    VideoCapture cap("/Users/yangenci/Desktop/Data/2.mp4");

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
            track.update_imgs(disparity.get_result(), leftFrame, count);
            if (track.target_exit()) {
               track.tracking();
                imshow("test~~", track.motion());
                analysis.initialize( leftFrame, disparity.get_result(), track.motion(), track.result() );
               track.show_path();
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
        skinExtract(leftFrame, Mat());

        waitKey(1);

    }
    return 0;
}
