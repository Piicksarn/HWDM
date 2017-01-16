#include <opencv2/opencv.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/calib3d.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class Camera{
private:
    Size borderSize;
    vector<string> fileList;
    Mat cameraMatrix;
    Mat rvec;
    Mat tvec;
    Mat distCoeffsMatrix;
    Mat resultImg;
    Size imgSize;
    vector<vector<Point2f>> srcPoints;
    vector<vector<Point3f>> dstPoints;
    vector<Point2f> imgPoint;
    vector<Point3f> objectPoint;
    void calRTvector();
public:
    void setFileList(int fileIndex);
    void Initialize(Size size, int index);
    void addChessboardPoints();
    void calibrate(Mat img);
    Mat getCameraMat();
    Mat getRvec();
    Mat getTvec();
    Mat getDistory();
    Mat getResultImg();

    // vector<Point3f> getObjectPoint();
    // vector<Point2f> getImgPoint();
    void calibrateAll();
    vector<vector<Point3f>> getObjectPoint();
    vector<vector<Point2f>> getImgPoint();
    Size getImgSize();
};
