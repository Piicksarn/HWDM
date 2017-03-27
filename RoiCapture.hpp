#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;
using namespace cv;

class Capture {
private:
    Mat disp_map;
    Mat source;
    Mat pre_image;
    Mat pre_disp;
    Mat old_disp;
    Mat old_image;
    Mat mask;
    int frame_index;
    void getNiceFgMask(Mat fgmask);
    Rect getNiceContour( Mat fgmask, Mat left, int i);
    void roi_check(Rect roi_d, Rect roi_src, Mat img);
    bool in_bound(Rect inner, Rect outer);
public:
    void initialize(Mat stereo, Mat left);
    void update_frame(Mat new_img, Mat new_disp);
    void cal_roi();
};

class ROI {
private:
    Point pos;
    double width;
    double height;
public:
    ROI(Point p, double w, double h);
    void update();
    Point get_pos();
    double get_width();
    double get_height();
};

class PWDS {
private:
    double sigma = 1;
    int window_size = 5;
    double key_gray_val = 0;
    Mat image;
    float cal_window(Mat window, int i);
    void density_estimate();
    float cal_gaussain(int xi, int x);
    vector<Point2f> gray_popu_val;
public:
    void set_image(Mat img);
    void get_key_pob();
    double get_low_bound();
    double get_up_bound();
};
