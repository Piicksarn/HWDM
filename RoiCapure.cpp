#include "Roicapture.hpp"

void Capture::initialize(Mat stereo, Mat left) {
     source = left;
     disp_map = stereo;
}

void Capture::update_frame(Mat new_img, Mat new_disp) {
    cvtColor(new_img, new_img, CV_BGR2GRAY);
    pre_disp = disp_map.clone();
    pre_image = source.clone();
    source = new_img.clone();
    disp_map = new_disp.clone();
}

void Capture::cal_roi() {
    Mat disp_diff, src_diff;
    absdiff(disp_map, pre_disp, disp_diff);
    absdiff(source, pre_image, src_diff);

    Mat temp = src_diff.clone();
    getNiceFgMask(disp_diff);
    getNiceFgMask(src_diff);

    Mat both_diff = disp_diff & src_diff;

    threshold(both_diff, both_diff, 20, 255, THRESH_BINARY);

    src_diff = src_diff & temp;
    imshow("both_diff", both_diff);
    imshow("src_diff", src_diff);
    imshow("disp_diff", disp_diff);

    getNiceContour(both_diff, src_diff, 1);
    threshold(src_diff, src_diff, 20, 255, THRESH_BINARY);

    getNiceContour(src_diff, src_diff, 2);
}

void Capture::getNiceFgMask(Mat fgmask) {
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    GaussianBlur(fgmask, fgmask, Size(3, 3), 1, 0);
    morphologyEx(fgmask, fgmask, MORPH_DILATE, element);
    GaussianBlur(fgmask, fgmask, Size(5, 5), 6, 0);
    morphologyEx(fgmask, fgmask, MORPH_ERODE, element);
    morphologyEx(fgmask, fgmask, MORPH_DILATE, element);
    GaussianBlur(fgmask, fgmask, Size(5, 5), 3, 0);
    morphologyEx(fgmask, fgmask, MORPH_ERODE, element);
    morphologyEx(fgmask, fgmask, MORPH_ERODE, element);
}
Mat Capture::getNiceContour(Mat fgmask, Mat left, int i) {
    // fgmask = fgmask & left;
    //    imshow("add", fgmask);
    getNiceFgMask(fgmask);
    // add(left, fgmask, fgmask);

       equalizeHist(fgmask, fgmask);
    //    imshow("equ",fgmask);
    int largest_area = 0;
    int largest_contour_index = 0;
    Rect bounding_rect;
    Mat dist = fgmask.clone(); // For the drawing contours.
    Mat imageROI = Mat(fgmask.size(), CV_8UC1, Scalar(0,0,0)); // For saving roi image.

    vector<vector<Point>> contours;
    findContours(fgmask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    vector<Rect> boundRect(contours.size());
    cvtColor(dist, dist, CV_GRAY2BGR);
    //    for (int i = 0; i < contours.size(); i++)
    //        if (contourArea(contours[i])) {
    //            drawContours(dist, contours, i, Scalar(255,4,4), CV_FILLED);
    //            imageROI = dist(boundingRect(contours[i]));
    //        }
    for(int i = 0; i < contours.size(); i++) // Iterate through each contour
    {
        double a = contourArea(contours[i], false); // Find the area of contour
        if(a > largest_area){
            largest_area = a;
            largest_contour_index = i; // Store the index of largest contour
            bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
            //            rectangle(dist, bounding_rect, Scalar(0,255,0), 1, 8, 0);

        }
    }
    rectangle(dist, bounding_rect, Scalar(0,255,0), 1, 8, 0);

    imshow("cont result"+to_string(i), dist);
    return dist;
}
