#include "Roicapture.hpp"

void PWDS::set_image(Mat img) {
    image = img;
    imshow("source", img);
    density_estimate();
}
void PWDS::density_estimate() {
    uchar *ptr = &image.at<uchar>(0, 0);
    vector<Point2f> gray_popu_val;
    float result = 0;
    for (int i = 0; i < 256; i++) {
        result = cal_window(image, i);

        gray_popu_val.push_back(Point2f((float)i, result));
        cout<< i<< " : "<< result<<endl;
    }
    float test = 0;
    float total = 0;
    int amount = 0;
    Mat showHistImg(100, 256, CV_8UC1, Scalar(255));
    for (int i = 0; i < gray_popu_val.size(); i++) {
        total += gray_popu_val[i].y;
    }
      for (int i = 0; i < gray_popu_val.size(); i++) {
//          float val = (gray_popu_val[i].y / total);
//          cout<<"val:"<<val<<endl;
          line(showHistImg, Point((int)i, (int)((1-gray_popu_val[i].y*10)*100)), Point((int)gray_popu_val[i].x, 99), Scalar(23, 120, 35));
//          test += val;
      }
      imshow("test", showHistImg);

    cout<<"(should be 1)total:"<<total<<endl;
}
float PWDS::cal_window(Mat window, int i) {
    uchar *ptr = &window.at<uchar>(0, 0);
    float sum = 0;
    int amount = 0;

    for (int c = 0; c < window.cols; c++) {
        for (int r = 0; r < window.rows; r++) {
            if ((int)(*ptr) != 0) {
              sum += cal_gaussain((int)(*ptr), i);
              amount++;
            }
            ptr++;
        }
    }
    amount -= 1;
    return sum / amount;
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
