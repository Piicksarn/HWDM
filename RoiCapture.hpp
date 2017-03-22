#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
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
