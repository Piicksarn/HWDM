//#include "PSO.hpp"
#include "Roicapture.hpp"

class Track {
private:
    Mat stereo;
    Mat gray_image;
    Mat color_image;
    vector<Mat> new_img;
    Target target;
    PSO pso;
    Capture capture;
    Rect track_result;
    Mat motion_img;
    int count = 0;
    int data_count = 0;
    vector<Point> path;
    bool good_roi(Rect pso, Rect active);
    float computRectJoinUnion(Rect rc1, Rect rc2);
public:
    void update_imgs(Mat s, Mat c, int count);
    void initial_target(Mat s, Mat c);
    void tracking();
    bool target_exit();
    void search_target();
    void show_path();
    Rect result();
    Mat motion();

};
