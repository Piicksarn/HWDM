#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace cv;
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
