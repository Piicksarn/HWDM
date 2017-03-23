#include "Roicapture.hpp"

void Capture::initialize(Mat stereo, Mat left) {
     source = left;
     disp_map = stereo;
     old_disp = stereo;
     old_image = stereo;
}

void Capture::update_frame(Mat new_img, Mat new_disp) {
    cvtColor(new_img, new_img, CV_BGR2GRAY);
    old_disp = pre_disp.clone();
    old_image = pre_image.clone();
    pre_disp = disp_map.clone();
    pre_image = source.clone();
    source = new_img.clone();
    disp_map = new_disp.clone();
}

void Capture::cal_roi() {
    Mat disp_diff_A, src_diff_A, disp_diff_B, src_diff_B;
    Mat disp_diff, src_diff;
    absdiff(disp_map, pre_disp, disp_diff_B);
    absdiff(source, pre_image, src_diff_B);

    absdiff(old_disp, pre_disp, disp_diff_A);
    absdiff(old_image, pre_image, src_diff_A);

    disp_diff = disp_diff_A & disp_diff_B;
    src_diff = src_diff_A & src_diff_B;

    Mat temp = src_diff.clone();

    threshold(src_diff, src_diff, 20, 255, THRESH_BINARY);
    threshold(disp_diff, disp_diff, 10, 255, THRESH_BINARY);
    equalizeHist(disp_diff, disp_diff);

    getNiceFgMask(disp_diff);
    getNiceFgMask(src_diff);

    Mat both_diff = disp_diff & src_diff;
    threshold(both_diff, both_diff, 20, 255, THRESH_BINARY);

    Rect disp_roi = getNiceContour(both_diff, pre_disp, 1);
    Rect src_roi = getNiceContour(src_diff, pre_disp, 2);

    roi_check(disp_roi, src_roi, pre_image);
}
void Capture::roi_check(Rect roi_d, Rect roi_src, Mat img) {
  /* 1. Check roi_d and roi_src exist.
   * 2. Check roi_d and roi_src area raio not less than 0.15
   * 3. Check roi_d and roi_src bouded each other
   * 4. Draw the roi_src on img
   */
    PWDS pwds;
    cvtColor(img, img, CV_GRAY2BGR);
    if(roi_d.area() != 0 && (roi_src.area()/roi_d.area()) < 7 && in_bound(roi_d, roi_src)) {
        rectangle(img, roi_d, Scalar(0, 255, 0), 1, 8, 0);
        rectangle(img, roi_src, Scalar(255, 255, 0), 1, 8, 0);
        pwds.set_image(img(roi_d));
    }
//    else {
//      rectangle(img, roi_d, Scalar(0, 0, 255), 1, 8, 0);
//      rectangle(img, roi_src, Scalar(0, 30, 255), 1, 8, 0);
//      circle(img, roi_d.tl(), 5, Scalar(0,255,0));
//    }
    imshow("roi_result", img);
}
bool Capture::in_bound(Rect inner, Rect outer) {
    if (inner.tl().x < outer.tl().x || inner.br().x > outer.br().x)
        return false;
    if (inner.tl().y < outer.tl().y || inner.br().y > outer.br().y)
        return false;
    return true;
}
void Capture::getNiceFgMask(Mat fgmask) {
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    GaussianBlur(fgmask, fgmask, Size(5, 5), 6, 0);
    morphologyEx(fgmask, fgmask, MORPH_DILATE, element);
    GaussianBlur(fgmask, fgmask, Size(5, 5), 6, 0);
    morphologyEx(fgmask, fgmask, MORPH_ERODE, element);
    morphologyEx(fgmask, fgmask, MORPH_DILATE, element);
    GaussianBlur(fgmask, fgmask, Size(5, 5), 3, 0);
    morphologyEx(fgmask, fgmask, MORPH_ERODE, element);
    morphologyEx(fgmask, fgmask, MORPH_ERODE, element);

}
Rect Capture::getNiceContour(Mat fgmask, Mat left, int i) {

    int largest_area = 0;
    int largest_contour_index = 0;
    Rect bounding_rect;
    vector<Vec4i> hierarchy;
    getNiceFgMask(fgmask);
    equalizeHist(fgmask, fgmask);
    Mat dist = fgmask.clone(); // For the drawing contours.
    Mat imageROI = Mat(fgmask.size(), CV_8UC1, Scalar(0,0,0)); // For saving roi image.

    vector<vector<Point>> contours;
//    findContours(fgmask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    findContours(fgmask, contours, CV_FILLED, CV_CHAIN_APPROX_NONE);

    vector<Rect> boundRect(contours.size());
    cvtColor(dist, dist, CV_GRAY2BGR);
    for(int i = 0; i < contours.size(); i++) // Iterate through each contour
    {
        double a = contourArea(contours[i], false); // Find the area of contour
        if(a > largest_area){
            largest_area = a;
            largest_contour_index = i; // Store the index of largest contour
            bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
        }
        // drawContours(pre_image, contours, i, Scalar(0, 0, 0), CV_FILLED, 8, hierarchy);
    }
    return bounding_rect;
}
