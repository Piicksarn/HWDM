#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <cmath>
using namespace std;
using namespace cv;

class Agent {
private:
    Point position;
    Point velocity;
    Point best_pos;
    double fitness;
    double best_val;
    double gray_thre_val;
    vector<int> goal_seq;
    vector<int> img_seq;
    Rect goal;
    Mat src;
    Mat foreground;
    double phi_1 = 0.3;
    double phi_2 = 0.7;
    void set_velocity();
    void set_sequence();
    Point bound_v(Point v);
    Point bound_x(Point x);
public:
  void set_position();
   void cal_fitness();
    void initialize(Rect goal_rect, Mat img, vector<int> g_seq, double threshold_val, Mat fore);
    void update(Point group_best);
    double get_fitness();
    Point get_position();
};
class PSO {
private:
    vector<int> goal;
    vector<Agent> agent_list;
    Point group_best;
    double gray_thre_val;
    double best_val;
    Rect target;
    Mat img;
    int amount = 90;
    int stereo_thre_val;
    int image_thre_val;
    bool find_out = true;
    vector<int> sequentialize(Mat img);
    void update();
    void find_best();
public:
    void initialize(Mat goal_region, Mat gray_img, Rect t, double threshold_val, Mat foreground);
    Rect get_result();
    void set_threshold(int stereo, int image);
    void training();
};
