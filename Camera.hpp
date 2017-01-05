#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class Camera{
private:
    Size borderSize;
    vector<string> fileList;
    Mat cameraMatrix;
    Mat rotationMatrix;
    Mat translateMatrix;
    Mat distCoeffsMatrix;
    Mat resultImg;


    vector<vector<Point2f> > srcPoints;
    vector<vector<Point3f> > dstPoints;

public:
    void setFileList(int fileIndex);
    void Initialize(Size size, int index);
    void addChessboardPoints();
    void calibrate(Mat img);
    Mat getCameraMat();
    Mat getRotationMat();
    Mat getTraslateMat();
    Mat getDistory();
    Mat getResultImg();
};
