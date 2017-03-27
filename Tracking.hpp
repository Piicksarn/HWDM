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
    double phi_1 = 0.3;
    double phi_2 = 0.7;
    void set_velocity();
    void set_sequence();
    Point bound_v(Point v);
    Point bound_x(Point x);
public:
  void set_position();
   Mat get_img();
   void cal_fitness();
    void initialize(Rect goal_rect, Mat img, vector<int> g_seq, double threshold_val);
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
    int iter_time = 1;
    int amount = 40;
    vector<int> sequentialize(Mat img);
    void update();
    void find_best();
public:
    void initialize(Mat img, Mat src, Rect t, double threshold_val);
    Rect get_result();
    void training();
};
