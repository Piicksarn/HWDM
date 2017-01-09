#include "Camera.hpp"

class Preprocedure{
private:
    Mat rightImage;
    Mat leftImage;
    Mat rvecOfTwo;
    Mat tvecOfTwo;
    Mat m_distOfTwo;
    Camera leftCamera;
    Camera rightCamera;
    Size imgSize;
    Mat R1, R2, P1, P2;
    void testStereoRectify();
    void testStereoCalibrate();
    void testFundamental();
public:
    void Initialize(Size size);
    void calibrate(Mat left, Mat right);
    void imageRectification();
    Mat getDisparityMap();
};
