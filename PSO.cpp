#include "headfile/PSO.hpp"

void PSO::initialize(Mat goal_region, Mat gray_img, Rect t, double threshold_val, Mat foreground) {
   // Calculate the value of standard goal
   best_val = (double)INT_MAX;
   gray_thre_val = threshold_val;
   goal = sequentialize(goal_region);
   target = t;
   srand( (unsigned)time(NULL));
   img = gray_img;
   imshow("fore ground", foreground);
   for (int i = 0; i < amount; i++) {
     Agent *agent = new Agent;
     agent->initialize(target, gray_img, goal, gray_thre_val,foreground);
     agent_list.push_back(*agent);
     delete agent;
   }
}
// void PSO::initialize(Target t) {
//    // Calculate the value of standard goal
//       best_val = (double)INT_MAX;
//       gray_thre_val = t.get_threshold()[1];
//       goal = sequentialize(t.get_gray_target());
//       target = t.get_target_rect();
//       img = t.get_gray_image();
//       Mat foreground = t.get_target_area();
//       srand( (unsigned)time(NULL));
//       for (int i = 0; i < amount; i++) {
//         Agent *agent = new Agent;
//         agent->initialize(target, img, goal, gray_thre_val,foreground);
//         agent_list.push_back(*agent);
//         delete agent;
//       }
//
// }

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
        // Mat t = img.clone();
        // cvtColor(t, t, CV_GRAY2BGR);
        if (best_val < 4406740 )
            break;
        if (best_val > 4406740 && counter >= 10) {
           break;
        }
//        rectangle(t, target, Scalar(0, 0, 255));
        for (int i = 0; i < agent_list.size(); i++) {
            // circle(t, agent_list[i].get_position(), 0.1, Scalar(0, 255, 255), 5, CV_AA, 0);

            agent_list[i].update(group_best);
            agent_list[i].cal_fitness();
        }
        counter++;
        // imshow("yee", t);
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
