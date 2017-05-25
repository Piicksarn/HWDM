#include "headfile/FingerAnalysis.hpp"

void Analysis::initialize( Mat s_img, Mat g_img, Mat m_img, Rect roi ) {
    imshow("gray", g_img);
    imshow("stereo", s_img);
    imshow("motion", m_img);
    
}
