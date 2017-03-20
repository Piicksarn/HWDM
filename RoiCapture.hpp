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
    void getNiceFgMask(Mat fgmask);
    Mat getNiceContour( Mat fgmask, Mat left, int i);
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
