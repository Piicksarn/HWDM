#include "headfile/PSO.hpp"

void Agent::initialize(Rect goal_rect, Mat img, vector<int> g_seq, double threshold_val, Mat fore) {
    goal = goal_rect;
    goal_seq = g_seq;
    src = fore;
    best_val = (double)INT_MAX;
    gray_thre_val = threshold_val;
    set_position();
    set_velocity();
    set_sequence();
    cal_fitness();
}

void Agent::set_position() {
    int g_x = goal.tl().x;
    int g_y = goal.tl().y;
    int f = rand() % 10;
    int x = rand() % goal.width;
    int y = rand() % goal.height;
    f > 5 ? (x *= -1, y *= -1) : (x = x, y = y);
    position = bound_x(Point(g_x + x, g_y + y));
}
void Agent::set_velocity() {
    int f = rand() % 10;
    int v_x = rand() % goal.width;
    int v_y = rand() % goal.height;
    f > 5 ? (v_x *= -1, v_y *= -1) : (v_x = v_x, v_y = v_y);
    velocity = bound_v(Point(v_x, v_y));
}
void Agent::set_sequence() {
    // Get the region of agent point using position
    // Mat img_s = src(Rect(position.x, position.y, goal.width, goal.height));
    // Mat img, mask;
    // threshold(img_s, mask, gray_thre_val, 255, THRESH_BINARY);
    // img_s.copyTo(img, mask);
    Mat img = src(Rect(position.x, position.y, goal.width, goal.height)).clone();
    // Set the sequence for the region of agents
    vector<int> list;
    uchar *ptr = &img.at<uchar>(0, 0);
    for (int r = 0; r < img.rows; r += 1) {
        for (int c = 0; c < img.cols; c += 1) {
            list.push_back((int)(*ptr));
            ptr++;
        }
    }
    img_seq = list;
}
void Agent::cal_fitness() {
    double sum = 0;
    for (int i = 0; i < img_seq.size(); i++) {
        sum += pow((img_seq[i] - goal_seq[i]), 2);
    }
    fitness =  0.5 * sum;
    if(fitness < best_val) {
        best_val = fitness;
        best_pos = position;
    }
}
void Agent::update(Point group_best) {
    int v_x = velocity.x + phi_2 * (group_best.x - position.x) + phi_1 * (best_pos.x - position.x);
    int v_y = velocity.y + phi_2 * (group_best.y - position.y) + phi_1 * (best_pos.y - position.y);
    velocity = bound_v(Point(v_x, v_y));
    position = bound_x(Point((position.x + velocity.x), (position.y + velocity.y)));
    set_sequence();
}
Point Agent::bound_v(Point v) {
    // High bound
    if(v.x > goal.width / 2)
        v.x = goal.width / 2;
    if(v.y > goal.height / 2 )
        v.y = goal.height / 2;
    // Low bound
    if(v.x < goal.width / 2 * -1)
        v.x = goal.width / 2 * -1;
    if(v.y < goal.height / 2 * -1)
        v.y = goal.height / 2 * -1;
    return v;
}
Point Agent::bound_x(Point x) {
    // Let searching area near by the target.
    int x_low_bound = goal.x - goal.width;
    int x_up_bound = goal.x + goal.width;
    int y_low_bound = goal.y - goal.height;
    int y_up_bound = goal.y + goal.height;

    // Let the searching area won't get over the image size.
    if(x_low_bound < 0)
      x_low_bound = 0;
    if(y_low_bound < 0)
      y_low_bound = 0;
    if(x_up_bound > src.cols - goal.width)
      x_up_bound = src.cols - goal.width;
    if(y_up_bound > src.rows - goal.height)
      y_up_bound = src.rows - goal.height;

    // Consider the position of agent.
    if(x.x > x_up_bound)
        x.x = x_up_bound;
    if(x.x < x_low_bound)
        x.x = x_low_bound;
    if(x.y > y_up_bound)
        x.y = y_up_bound;
    if(x.y < y_low_bound)
        x.y = y_low_bound;

    return x;
}
Point Agent::get_position() {
    return position;
}
double Agent::get_fitness() {
    return fitness;
}
