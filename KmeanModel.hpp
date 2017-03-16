#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
using std::vector;
using namespace cv;
using namespace std;
class Kmeans{
private:
    int k = 3;
    vector<vector<int>> dataList;
    vector<vector<int>> wList;
    vector<int> loadData(Mat img, vector<int> data);
    vector<vector<vector<int>>> clusterList;
    vector<vector<int>> cluster1;
    vector<vector<int>> cluster2;
    vector<vector<int>> cluster0;
    void assign_cluster(vector<int> dist, vector<int> data);
    void update_center();
    int run_time = 100;
public:
    void initialize();
    void setData(Mat left, Mat stereo, Mat diff_left, Mat diff_stereo);
    void calculate_dist();
    void run_clustering();
};
