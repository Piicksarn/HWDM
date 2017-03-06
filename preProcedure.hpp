#include "Camera.hpp"

class Preprocedure{
private:
    Mat rvecOfTwo;
    Mat tvecOfTwo;
    Mat m_distOfTwo;
    Camera leftCamera;
    Camera rightCamera;
    Size imgSize;
    Mat R1, R2, P1, P2;
    int status;
    Mat leftCam;
    Mat leftDis;
    Mat rightCam;
    Mat rightDis;
    Mat leftResult;
    Mat rightResult;
    void testStereoRectify(Mat left, Mat right);
    void testStereoCalibrate();
    void writeParaFile();
    void readPareFile();
public:
    void Initialize(Size size, Mat left, Mat right);
    void CameraCalibrate();
    void imageRectification();
    Mat getRight();
    Mat getLeft();
};
