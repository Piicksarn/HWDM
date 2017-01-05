#include "Camera.hpp"

class Preprocedure{
private:
    Size m_borderSize;
    Mat rightImage;
    Mat leftImage;
    Mat cameraMat;
    Mat m_rvecOfTwo;
    Mat m_tvecOfTwo;
    Mat m_distOfTwo;
    Camera leftCamera;
    Camera rightCamera;

public:
    void Initialize(Size size);
    void calibrate(Mat left, Mat right);
    Mat getDisparityMap();
};
