#include "headfile/Target.hpp"

void Target::set_images(Mat s, Mat c) {
    stereo = s.clone();
    color_image = c.clone();
    cvtColor(c, gray_image, CV_BGR2GRAY);
}
void Target::update_frame(Mat s, Mat c) {
  stereo = s.clone();
  color_image = c.clone();
  cvtColor(c, gray_image, CV_BGR2GRAY);
}
void Target::set_target(Rect t) {
    target_rec= t;
}
void Target::set_threshold(double stereo, double gray) {
    stereo_thre_val = stereo;
    gray_thre_val = gray;
}

void Target::refine_target() {
    Mat ste = stereo.clone();
    Mat result;
    threshold(ste, ste, stereo_thre_val, 255, THRESH_BINARY);
    gray_image.copyTo(result, ste);
    threshold(result, mask, gray_thre_val, 255, THRESH_BINARY);
    get_skin();
    // refine_position();
    refine_threshold();
}

Mat Target::get_target_area() {
    Mat result;
    gray_image.copyTo(result, mask);
    return result;
}

void Target::get_skin() {
    Mat YCbCr;
    vector<Mat> planes;
    Mat frame = color_image.clone();
    Mat color;
    frame.copyTo(color, mask);
    frame = color;
    cvtColor(frame, YCbCr, CV_RGB2YCrCb);

    // Skin color: Y: 0 ~255 | Cr: 133 ~ 177 | Cb: 98 ~ 122
    inRange(YCbCr, Scalar(0, 77, 133), Scalar(255, 127, 173), YCbCr);

    Mat result = YCbCr.clone();
    blur(result, result, Size(3, 3));
    Mat element = getStructuringElement(MORPH_CROSS, Size(5,5), Point(2, 2));
    morphologyEx(result, result, MORPH_ERODE, element);
    morphologyEx(result, result, MORPH_DILATE, element);
    morphologyEx(result, result, MORPH_DILATE, element);
    morphologyEx(result, result, MORPH_ERODE, element);
    mask = result;
}

void Target::refine_position() {
    vector<Vec4i> hierarchy;
    int max_area = 0;
    vector<vector<Point>> contours;
    Rect bounding_rect;
    findContours(mask, contours, CV_FILLED, CV_CHAIN_APPROX_NONE);
    for(int i = 0; i < contours.size(); i++)
    {
        double a = contourArea(contours[i], false);
        Rect r = boundingRect(contours[i]);
        double d = norm(r.tl()-target_rec.tl());
        if(a > max_area && d < 15){
            max_area = a;
            bounding_rect = r;
            target_rec= bounding_rect;
        }
    }

}

void Target::refine_threshold() {
    stereo_thre_val = mean(stereo(target_rec))[0];
    pwds.set_image(gray_image(target_rec), 0.9);
    gray_thre_val = pwds.get_key_val();
}

vector<int> Target::get_threshold() {
    vector<int> threshold;
    threshold.push_back(stereo_thre_val);
    threshold.push_back(gray_thre_val);
    return threshold;
}

Mat Target::get_gray_target() {
   Mat result;
   gray_image.copyTo(result, mask);
   return result(target_rec);
}

Rect Target::get_target_rect() {
  return target_rec;
}

Mat Target::get_gray_image() {
  return gray_image;
}

Mat Target::get_mask() {
  return mask;
}
