#include "Tracking.hpp"

void PSO::initialize(Mat goal_region, Mat gray_img, Rect t, double threshold_val, Mat foreground) {
    // Calculate the value of standard goal
    best_val = (double)INT_MAX;
    gray_thre_val = threshold_val;
    goal = sequentialize(goal_region);
    target = t;
    srand( (unsigned)time(NULL));
    img = gray_img;
    imshow("gray", img);
    imshow("fore ground", foreground);
    for (int i = 0; i < amount; i++) {
      Agent *agent = new Agent;
      agent->initialize(target, gray_img, goal, gray_thre_val,foreground);
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
        if (best_val < 13500000 )
            break;
        if (best_val > 13500000 && counter >= 10) {
           break;
        }

        for (int i = 0; i < agent_list.size(); i++) {
            agent_list[i].update(group_best);
            agent_list[i].cal_fitness();
        }
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
