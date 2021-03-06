/*
 *  Created by Piicksarn.
 *  Copyright © 2017 Piicksarn.Yang. All rights reserved.
 */
 #include "opencv2/ximgproc/disparity_filter.hpp"
 #include "opencv2/calib3d.hpp"
 #include "opencv2/imgproc.hpp"
 #include "opencv2/imgcodecs.hpp"
 #include "opencv2/highgui.hpp"
 #include "opencv2/core/utility.hpp"
 using namespace cv;
using namespace std;

 class Disparity{
 private:
   Mat left_img;
   Mat right_img;
   void compute_disp();
   void optimization();
   void filtering_rough();
   void filtering_detail();
   Mat detail_result;
   Mat rough_result;
 public:
   void initialize(Mat left, Mat right);
   Mat get_result();
 };
