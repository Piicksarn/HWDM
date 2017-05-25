#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "PSO.hpp"

using namespace std;
using namespace cv;

class Capture {
private:
    Target target_element;
    Target target_ele_test;
    Mat disp_map;
    Mat pre_color;
    Mat source;
    Mat pre_image;
    Mat pre_disp;
    Mat old_disp;
    Mat old_image;
    Mat mask;
    Mat target;
    Mat gray_diff;
    Mat diff_in_all;
    Rect tar_roi;
    int frame_index;
    int stereo_thre_val;
    int image_thre_val;
    vector<Point> path;
    vector<Rect> roiList;
    int roi_amount = 0;
    Rect region_rect;
    Mat region_mask;
    Mat motion;
    void getNiceFgMask(Mat fgmask);
    Rect getNiceContour( Mat fgmask, Mat left, int i);
    void roi_check(Rect roi_d, Rect roi_src, Mat src);
    Mat set_target(Mat mask, Rect Roi, Mat src);
    Mat get_foreground();
    bool is_target(Rect roi_d);
    Rect update_roi(Rect roi);
    Mat cal_diff();
    Mat get_region_mask();
    Point3f get_inner_Circle(Mat region);
    int stereo_prob(Rect region, Mat stereo_prob_image);
    void proto_test();
    void update_target(Rect new_target);
public:
    void initialize(Mat stereo, Mat left);
    void update_frame(Mat new_img, Mat new_disp);
    void cal_roi();
    bool target_exit();
    Mat get_mask();
    Target get_target();
    void do_tracking();
    void set_Target(Target t);
    Rect get_result();
    void cal_search_region();
    Rect get_region_pos();
    Mat get_motion();

};
