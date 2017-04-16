#include "Tracking.hpp"

void PSO::initialize(Mat img, Mat src, Rect t, double threshold_val) {
    // Calculate the value of standard goal
    best_val = (double)INT_MAX;
    gray_thre_val = threshold_val;
    goal = sequentialize(img);
    target = t;
    srand( (unsigned)time(NULL));

    for (int i = 0; i < amount; i++) {
      Agent *agent = new Agent;
      agent->initialize(target, src, goal, gray_thre_val);
      agent_list.push_back(*agent);
      delete agent;
    }
}
vector<int> PSO::sequentialize(Mat img) {
    vector<int> list;
    uchar *ptr = &img.at<uchar>(0, 0);
    for (int r = 0; r < img.rows; r += 1) {
        for (int c = 0; c < img.cols; c += 1) {
            list.push_back((int)(*ptr));
            ptr++;
        }
    }
    return list;
}
void PSO::training() {
    int counter = 0;
    while(1) {
        find_best();
        for (int i = 0; i < agent_list.size(); i++) {
            agent_list[i].update(group_best);
            agent_list[i].cal_fitness();
        }
        if (best_val < 13500000)
            break;
        counter++;
    }
}
void PSO::find_best() {
  for (int i = 0; i < agent_list.size(); i++) {
      if (agent_list[i].get_fitness() < best_val) {
          group_best = agent_list[i].get_position();
          best_val = agent_list[i].get_fitness();
      }
  }
}

Rect PSO::get_result() {
    return Rect(group_best.x, group_best.y, target.width, target.height);
}
