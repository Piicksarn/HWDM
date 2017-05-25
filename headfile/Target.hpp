#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "PWDS.hpp"
using namespace std;
using namespace cv;
class Target {
private:
   Mat color_image;
   Mat gray_image;
   Mat stereo;
   Mat mask;
   double stereo_thre_val;
   double gray_thre_val;
   Rect target_rec;
   PWDS pwds;
   void get_skin();
   void refine_position();
   void refine_threshold();
public:
  void refine_target();
   void set_images(Mat s, Mat c);
   void set_target(Rect t);
   void set_threshold(double stereo, double gray);
   Rect get_target();
   Mat get_target_area();
   vector<int> get_threshold();
   Mat get_gray_target();
   Rect get_target_rect();
   Mat get_gray_image();
   Mat get_mask();
   void update_frame(Mat s, Mat c);

};
