#include "headfile/Roicapture.hpp"
void Capture::initialize(Mat stereo, Mat left) {
    source = left;
    disp_map = stereo;
    old_disp = stereo;
    old_image = stereo;
}

void Capture::update_frame(Mat new_img, Mat new_disp) {
    pre_color = new_img;
    cvtColor(new_img, new_img, CV_BGR2GRAY);
    old_disp = pre_disp.clone();
    old_image = pre_image.clone();
    pre_disp = disp_map.clone();
    pre_image = source.clone();
    source = new_img.clone();
    disp_map = new_disp.clone();
}
Mat Capture::cal_diff() {
    Mat disp_diff_A, src_diff_A, disp_diff_B, src_diff_B;
    Mat disp_diff, src_diff;

    // Get the different pixel between time: t-1, t, t
    absdiff(disp_map, pre_disp, disp_diff_B);
    absdiff(source, pre_image, src_diff_B);

    absdiff(old_disp, pre_disp, disp_diff_A);
    absdiff(old_image, pre_image, src_diff_A);

    disp_diff = disp_diff_A & disp_diff_B;
    src_diff = src_diff_A & src_diff_B;

    // Getting the mask of the region of interest
    threshold(src_diff, src_diff, 5, 255, THRESH_BINARY);
    threshold(disp_diff, disp_diff, 5, 255, THRESH_BINARY);
    equalizeHist(disp_diff, disp_diff);

    // Globel variable for the difference between frames.
    diff_in_all = disp_diff & src_diff;
    gray_diff = src_diff;

    Mat test = disp_diff & src_diff;
    getNiceFgMask(test);
    return test;
}
void Capture::proto_test() {
    Mat both_diff = diff_in_all.clone();
    Mat result_mask = gray_diff.clone();
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    GaussianBlur(result_mask, result_mask, Size(3, 3), 6, 0);
    morphologyEx(result_mask, result_mask, MORPH_DILATE, element);
    morphologyEx(result_mask, result_mask, MORPH_ERODE, element);


    // Find biggest contour
    int largest_area = 0;
    Rect bounding_rect;
    vector<Vec4i> hierarchy;
    Mat dist = result_mask.clone(); // For the drawing contours.
    vector<vector<Point>> contours;
    motion = result_mask;
    findContours(result_mask, contours, CV_FILLED, CV_CHAIN_APPROX_NONE);
    cvtColor(dist, dist, CV_GRAY2BGR);
    for(int i = 0; i < contours.size(); i++) // Iterate through each contour
    {
        Rect r = boundingRect(contours[i]);

        if(stereo_prob(r, both_diff) > largest_area){
            largest_area = stereo_prob(r, both_diff);
            bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
            bounding_rect = r;
        }
    }
    region_rect = bounding_rect;
}

Point3f Capture::get_inner_Circle(Mat region) {
    Mat r = region.clone();
    Point3f p;
    return p;
}
int Capture::stereo_prob(Rect region, Mat stereo_prob_image) {
    Mat sqMat;
    Mat integral_img;
    integral(stereo_prob_image(region), integral_img, sqMat);
    return integral_img.at<int>(integral_img.rows-1, integral_img.cols-1) / 255;
}
void Capture::cal_roi() {

    Mat both_diff = cal_diff();
    threshold(both_diff, both_diff, 20, 255, THRESH_BINARY);
    Mat result_mask = both_diff.clone();
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    morphologyEx(result_mask, result_mask, MORPH_DILATE, element);
    morphologyEx(result_mask, result_mask, MORPH_ERODE, element);
    morphologyEx(result_mask, result_mask, MORPH_DILATE, element);
    morphologyEx(result_mask, result_mask, MORPH_DILATE, element);
    morphologyEx(result_mask, result_mask, MORPH_ERODE, element);

    mask = pre_disp.clone();
    Mat dst;
    mask.copyTo(dst, result_mask);
    Rect disp_roi = getNiceContour(both_diff, pre_disp, 1);
    mask = dst;

    roi_check(disp_roi, Rect(), result_mask);
}
void Capture::set_Target(Target t) {
    target_ele_test = t;
}
void Capture::do_tracking() {
    Mat search_image = get_foreground();
    PSO pso;
    Target t = target_ele_test;
    Mat target2 = t.get_gray_target();
    Mat pre_image2 = t.get_gray_image();
    Rect tar_roi2 = t.get_target_rect();
    Mat search_image2 = t.get_target_area();

    pso.initialize(target, pre_image2, tar_roi, image_thre_val, search_image2);
    pso.training();
    tar_roi = pso.get_result();
    Mat s = pre_color.clone();
    rectangle(s, pso.get_result(),Scalar(0, 0, 255));
}
void Capture::roi_check(Rect roi_d, Rect roi_src, Mat src) {
    PWDS pwds;
    Mat fore_mask;
    Mat mask2 = src.clone();
    Mat img = pre_disp.clone();
    Mat stereo = pre_disp.clone();
    cvtColor(img, img, CV_GRAY2BGR);

    vector<Vec4i> hierarchy;
    int largest_area = 0;
    vector<vector<Point>> contours;
    Rect bounding_rect;
    if(is_target(roi_d)) {
        // Get the threshold value of the stereo image and gray image
        pwds.set_image(mask(roi_d), 0.55);
        fore_mask = pwds.get_foreground(mask(roi_d));
        stereo_thre_val = pwds.get_key_val();
        threshold(fore_mask, fore_mask, 10, 255, THRESH_BINARY);
        pwds.set_image(set_target(fore_mask, roi_d, pre_image), 0.85);
        fore_mask = pwds.get_foreground(set_target(fore_mask, roi_d, pre_image));
        threshold(fore_mask, fore_mask, 10, 255, THRESH_BINARY);
        image_thre_val = pwds.get_key_val();

        // // Set the target position and the target image
        Mat search_image = get_foreground();
        Mat tmp = search_image.clone();
        findContours(search_image, contours, CV_FILLED, CV_CHAIN_APPROX_NONE);
        for(int i = 0; i < contours.size(); i++)
        {
            double a = contourArea(contours[i], false);
            if(a > largest_area){
                largest_area = a;
                bounding_rect = boundingRect(contours[i]);
            }
        }
        tar_roi = bounding_rect;
        // imshow("initial target", tmp(bounding_rect));
        // cvtColor(mask2, mask2, CV_GRAY2BGR);
        // rectangle(mask2, roi_d, Scalar(0,255,0));
        // imshow("MASK",mask2);
        target = set_target(tmp(bounding_rect), bounding_rect, pre_image);
        target_element.set_images(pre_disp, pre_color);
        target_element.set_target(tar_roi);
        target_element.set_threshold(stereo_thre_val, image_thre_val);
    }
}
bool Capture::is_target(Rect roi_d) {
    roiList.push_back(roi_d);
    double d = norm(roiList[roiList.size()-1].tl()-roiList[roiList.size()-2].tl());
    Mat img = pre_image.clone();
    rectangle(img, roi_d, Scalar(0, 255, 0), 1, 8, 0);


    if (mean(mask(roi_d))[0] > mean(pre_disp)[0] * 1 / 2 && d < 15)
        roi_amount++;
    else
        roi_amount = 0;

    // Return the result
    if (roi_amount >= 4)
        return true;
    else
        return false;
}
Mat Capture::set_target(Mat mask, Rect Roi, Mat src) {
    threshold(mask, mask, 10, 255, THRESH_BINARY);
    Mat result;
    Mat image = src(Roi).clone();
    image.copyTo(result, mask);
    return result;
}

Mat Capture::get_foreground() {
    Mat stereo = pre_disp.clone();
    Mat image = pre_image.clone();
    Mat result;
    threshold(stereo, stereo, stereo_thre_val, 255, THRESH_BINARY);
    image.copyTo(result, stereo);
    threshold(result, result, image_thre_val, 255, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    morphologyEx(result, result, MORPH_ERODE, element);
    morphologyEx(result, result, MORPH_DILATE, element);
    morphologyEx(result, result, MORPH_DILATE, element);
    morphologyEx(result, result, MORPH_ERODE, element);
    image.copyTo(result, result);
    return result;
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

bool Capture::target_exit() {
    if(target.empty())
        return false;
    else
        return true;
}

Mat Capture::get_mask() {
    return get_foreground();
}
Target Capture::get_target() {
    return target_element;
}
Rect Capture::get_result() {
    return tar_roi;
}
void Capture::cal_search_region() {
    cal_diff();
    proto_test();
}
Rect Capture::get_region_pos() {
    return region_rect;
}
Mat Capture::get_region_mask() {
    return region_mask;
}
Mat Capture::get_motion() {
    return motion;
}
