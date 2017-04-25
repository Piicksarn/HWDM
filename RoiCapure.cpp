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
    Mat result3;
    hconcat(src_diff, disp_diff, result3);
    hconcat(result3, both_diff, result3);
    imshow("different", result3);

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
        stereo_thre_val = pwds.get_key_val();
        threshold(fore_mask, fore_mask, 10, 255, THRESH_BINARY);
        pwds.set_image(set_target(fore_mask, roi_d, pre_image), 0.85);
        fore_mask = pwds.get_foreground(set_target(fore_mask, roi_d, pre_image));
        threshold(fore_mask, fore_mask, 10, 255, THRESH_BINARY);
        target = set_target(fore_mask, roi_d, pre_image);
        tar_roi = roi_d;
        thre_val = pwds.get_key_val();
        image_thre_val = pwds.get_key_val();
    }
}

Mat Capture::set_target(Mat mask, Rect Roi, Mat src) {
    threshold(mask, mask, 10, 255, THRESH_BINARY);
    Mat result;
    Mat image = src(Roi).clone();
    image.copyTo(result, mask);
    return result;
}

void Capture::do_tracking() {
    Mat search_image = get_foreground();
//     if(target.empty())
//         target = search_image(tar_roi);
     PSO pso;
     pso.initialize(target, pre_image, tar_roi, thre_val, search_image);
     pso.training();
//    Mat integral_image;
//    integral(search_image, integral_image);

     update_target(pso.get_result());
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
    imshow("dst", result);

    return result;
}
void Capture::update_target(Rect new_target) {

    Mat stereo = pre_disp(new_target);
    Mat image = pre_image(new_target);
    imshow("test-stereo", stereo);
    imshow("test-image", image);
    imshow("stereo", pre_disp);


     Mat dst;



     //end of test
     PWDS pwds1;
     pwds1.set_image(stereo, 0.4);
     stereo_thre_val = pwds1.get_key_val();
     threshold(stereo, stereo, pwds1.get_key_val(), 255, THRESH_BINARY);
     image.copyTo(dst, stereo);
     pwds1.set_image(dst, 0.8);
     image_thre_val = pwds1.get_key_val();
     threshold(dst, dst, image_thre_val, 255, THRESH_BINARY);


    Mat result = dst.clone();
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    morphologyEx(result, result, MORPH_DILATE, element);
    morphologyEx(result, result, MORPH_ERODE, element);

    morphologyEx(result, result, MORPH_DILATE, element);
    morphologyEx(result, result, MORPH_ERODE, element);
    imshow("result", result);

    // test for efficient of the threshold of both threshold value;
    Mat t = pre_disp.clone();
    Mat a = pre_image.clone();
    Mat d;
    Mat g;
    Mat e;
    threshold(t, t, stereo_thre_val, 255, THRESH_BINARY);
    a.copyTo(d, t);
    threshold(d, d, image_thre_val, 255, THRESH_BINARY);
    hconcat(t, d, e);
    a.copyTo(g, d);
    hconcat(e, g, g);

    Mat show = pre_image.clone();
    Moments mo = moments(g(tar_roi));
    Point center = Point(mo.m10/mo.m00 , mo.m01/mo.m00);
    cvtColor(g,g, CV_GRAY2BGR);
    path.push_back(Point(center.x + tar_roi.tl().x, center.y + tar_roi.tl().y));



    imshow("yee", g);

    tar_roi = new_target;
//    target = result(tar_roi);

    cvtColor(show, show, CV_GRAY2BGR);
    rectangle(show, new_target, Scalar(255, 255, 0), 1, 8, 0);
    imshow("test", show);


    if (path.size()>10)
        path.erase(path.begin());
    Mat router(pre_image.rows, pre_image.cols, CV_8UC1, Scalar(255));
    cvtColor(router, router, CV_GRAY2BGR);
    for (int i = 0; i < path.size(); i++) {
        circle(router, path[i], 3, Scalar(255-20*i, 255-10*i, 255-10*i), 5, CV_AA, 0);
    }
    imshow("path", router);


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
