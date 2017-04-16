#include "Roicapture.hpp"
void Capture::initialize(Mat stereo, Mat left) {
    source = left;
    disp_map = stereo;
    old_disp = stereo;
    old_image = stereo;
    imshow("test-stereo", stereo);
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
    threshold(src_diff, both_diff, 200, 255, THRESH_BINARY);
    mask = pre_disp.clone();
    mask.copyTo(both_diff, both_diff);
    mask = both_diff;

    if(target.empty()) {
        roi_check(disp_roi, src_roi, pre_disp);
    }
    else {
        do_tracking();
    }



    // if(src_roi.area() > 0) {
    // Mat test = pre_image.clone();
    // test = test(Rect(src_roi));
    // testk(test);
    // }

}

void Capture::testk(Mat src) {
    Mat img = src.clone();
    cvtColor(img, img, CV_GRAY2BGR);
    pyrMeanShiftFiltering(img, img, 30, 20, 3);
    imshow("result66", img );

    //    cout<<src.channels();
}
void Capture::roi_check(Rect roi_d, Rect roi_src, Mat src) {
    PWDS pwds;
    Mat fore_mask;
    Mat img = src.clone();
    cvtColor(img, img, CV_GRAY2BGR);
    if(roi_d.area() != 0 && (roi_src.area()/roi_d.area()) < 7 && in_bound(roi_d, roi_src)) {
        rectangle(img, roi_d, Scalar(0, 255, 0), 1, 8, 0);
        rectangle(img, roi_src, Scalar(255, 255, 0), 1, 8, 0);
        pwds.set_image(mask(roi_d), 0.55);
        fore_mask = pwds.get_foreground(mask(roi_d));
        threshold(fore_mask, fore_mask, 10, 255, THRESH_BINARY);
        pwds.set_image(set_target(fore_mask, roi_d, pre_image), 0.85);
        fore_mask = pwds.get_foreground(set_target(fore_mask, roi_d, pre_image));
        threshold(fore_mask, fore_mask, 10, 255, THRESH_BINARY);
        target = set_target(fore_mask, roi_d, pre_image);
        tar_roi = roi_d;
        thre_val = pwds.get_key_val();
    }
    // imshow("roi_result", set_target(fore_mask, roi_d, pre_image));
}

Mat Capture::set_target(Mat mask, Rect Roi, Mat src) {
    threshold(mask, mask, 10, 255, THRESH_BINARY);
    Mat result;
    Mat image = src(Roi).clone();
    image.copyTo(result, mask);
    return result;
}

void Capture::do_tracking() {
    PSO pso;
    pso.initialize(target, pre_image, tar_roi, thre_val);
    pso.training();
    update_target(pso.get_result());
}
void Capture::update_target(Rect new_target) {
    imshow("test-stereo", pre_disp);
    imshow("test-image", pre_image);

/* ===========================================
    the next target ---

==============================================*/
    Mat roi = pre_image(new_target);
    Mat test = pre_disp(new_target);
    Mat img, mask;
    threshold(roi, mask, thre_val, 255, THRESH_BINARY);
    pre_image(new_target).copyTo(img, mask);
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    imshow("img--~",img);
    morphologyEx(img, img, MORPH_ERODE, element);
    imshow("img--~ erode",img);
    morphologyEx(img, img, MORPH_DILATE, element);
    imshow("img--~ dilate",img);



    // Calculate PWDS of roi
    PWDS pwds;
    pwds.set_image(img, 0.8);
    cout<<"done setting"<<endl;
    //    img = pwds.get_foreground(img);
    thre_val = pwds.get_key_val();
    //    pre_image(new_target).copyTo(img, mask);
    cout<<"done set value"<<endl;
    img = pwds.get_foreground(img);
    threshold(img, mask, thre_val, 255, THRESH_BINARY);
    pre_image(new_target).copyTo(img, mask);

    tar_roi = new_target;
    target = img;

    Mat show = pre_image.clone();
    cvtColor(show, show, CV_GRAY2BGR);
    rectangle(show, new_target, Scalar(255, 255, 0), 1, 8, 0);
    imshow("test", show);
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
    }
    drawContours(fgmask, contours, largest_contour_index, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
    return bounding_rect;
}
