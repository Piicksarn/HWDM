#include "Roicapture.hpp"

void PWDS::set_image(Mat img) {
    image = img;
    density_estimate();
}
void PWDS::density_estimate() {
  uchar *ptr = &image.at<uchar>(0, 0);
  vector<Point2f> gray_popu_val;
  float result = 0;
    for (int c = 0; c < image.cols; c ++) {
      for (int r = 0; r < image.rows; r ++) {
        result = cal_window(image, ptr);
        result /= image.cols * image.rows;
        gray_popu_val.push_back(Point2f((float)(*ptr), result));
        ptr++;
      }
    }
}
float PWDS::cal_window(Mat window, uchar *x) {
    uchar *ptr = &window.at<uchar>(0, 0);
    float sum = 0;
    for (int c = 0; c < window.cols; c++) {
      for (int r = 0; r < window.rows; r++) {
        if (ptr != x) {
          sum += cal_gaussain((int)(*ptr), (int)(*x));
        }
        ptr++;
      }
    }
    return sum;
}

double PWDS::get_low_bound() {
  return 0;
}

double PWDS::get_up_bound() {
  return 0;
}

float PWDS::cal_gaussain(int xi, int x) {
  double result = exp(-0.5 * pow((x - xi), 2) / pow(sigma, 2));
  return result / (sqrt(2 * M_PI) * sigma);
}

double PWDS::get_pob() {

    return 0;
}
