#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "Tracking.hpp"
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
    Mat target;
    Rect tar_roi;
    Mat diff;
    int frame_index;
    double thre_val;
    int stereo_thre_val;
    int image_thre_val;
    vector<Point> path;
    vector<Rect> roiList;
    int roi_amount = 0;
    void getNiceFgMask(Mat fgmask);
    Rect getNiceContour( Mat fgmask, Mat left, int i);
    void roi_check(Rect roi_d, Rect roi_src, Mat src);
    bool in_bound(Rect inner, Rect outer);
    Mat merge_fore(Mat mask, Rect Roi, Mat src);
    void set_target(Rect roi, Mat image);
    Mat get_foreground();
    Mat get_diff();
    bool is_target(Rect roi_d);
    Rect update_roi(Rect roi);
    void update_target(Rect new_target);
public:
    void initialize(Mat stereo, Mat left);
    void update_frame(Mat new_img, Mat new_disp);
    void cal_roi();
    void do_tracking();
    bool target_exit();
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
    double bound = 0;
    Mat image;
    float cal_window(Mat window, int i);
    void density_estimate();
    float cal_gaussain(int xi, int x);
    vector<Point2f> gray_popu_val;
    Mat morphology_fore(Mat src);
  public:
    void set_image(Mat img, double bound_val);
    void set_target_bg(Mat src);
    void get_key_pob();
    double get_low_bound();
    double get_up_bound();
    Mat get_foreground(Mat input_img);
    double get_key_val();
};
