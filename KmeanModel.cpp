#include "KmeanModel.hpp"
void Kmeans::initialize() {
    srand((unsigned)time(NULL));
    int k = rand() % 255;
    vector<int> w1;
    vector<int> w2;
    vector<int> w3;
    for (int i = 0; i < 200; i++) {
        w1.push_back(rand() % 255);
        w2.push_back(rand() % 255);
        w3.push_back(rand() % 255);
    }
    for (int i = 0; i < 200; i++) {
        w1.push_back(rand() % 255);
        w2.push_back(0);
        w3.push_back(rand() % 255);
    }
    wList.push_back(w1);
    wList.push_back(w2);
    wList.push_back(w3);
}
void Kmeans::setData(Mat left, Mat stereo, Mat diff_left, Mat diff_stereo) {
    vector<int> data;
    for (int r = 0; r < left.rows; r += 10) {
        for (int c = 0; c < left.cols; c += 10) {
            data = loadData(left(Rect(c, r, 10, 10)), data);
            data = loadData(stereo(Rect(c, r, 10, 10)), data);
            data = loadData(diff_left(Rect(c, r, 10, 10)), data);
            data = loadData(diff_stereo(Rect(c, r, 10, 10)), data);
            dataList.push_back(data);
            data.clear();
        }
    }
}

vector<int> Kmeans::loadData(Mat img, vector<int> data) {
    uchar *ptr = &img.at<uchar>(0, 0);
    for (int r = 0; r < img.rows; r += 1) {
        for (int c = 0; c < img.cols; c += 1) {
            data.push_back((int)(*ptr));
            ptr++;
        }
    }
    return data;
}
void Kmeans::run_clustering() {

    for (int i = 0; i < run_time; i++) {
        calculate_dist();
        update_center();
    }
}
void Kmeans::calculate_dist() {
    int minDist = 0;
    vector<int> distance;
    for (int i = 0; i < dataList.size(); i++) {
        vector<int> data = dataList[i];
        //calculate distance between data and w.
        int sum1 = 0;
        int sum2 = 0;
        int sum3 = 0;
        for (int j = 0; j < dataList[i].size(); j++) {
            sum1 += abs(data[j] - wList[0][j]);
            sum2 += abs(data[j] - wList[1][j]);
            sum3 += abs(data[j] - wList[2][j]);
        }
        distance.push_back(sum1);
        distance.push_back(sum2);
        distance.push_back(sum3);
        assign_cluster(distance, data);
        distance.clear();
    }
    clusterList.clear();
    clusterList.push_back(cluster0);
    clusterList.push_back(cluster1);
    clusterList.push_back(cluster2);
}

void Kmeans::assign_cluster(vector<int> dist, vector<int> data) {
    int min = 99999999999;
    int index = 0;
    
    for (int i = 0; i < dist.size(); i++) {
        if (dist[i] < min) {
            min = dist[i];
            index = i;
        }
    }
    if (index == 0)
        cluster0.push_back(data);
    else if (index == 1)
        cluster1.push_back(data);
    else
        cluster2.push_back(data);
}

void Kmeans::update_center() {
    for (int index = 0; index < clusterList.size(); index++) {
        vector<vector<int>> cluster = clusterList[index];
        
    }
}
