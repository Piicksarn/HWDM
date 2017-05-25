#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class Analysis {
private:
    Mat stereo;
    Mat gray;
    Mat motion;
    Rect target;
public:
    void initialize( Mat s_img, Mat g_img, Mat m_img, Rect roi );

};
