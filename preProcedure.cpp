/*
 *  Created by Piicksarn.
 *  Copyright © 2017 Piicksarn.Yang. All rights reserved.
 */
#include "preProcedure.hpp"

 void Preprocedure::Initialize(Size size){
     leftCamera.Initialize(Size(9,6), 1);
     rightCamera.Initialize(Size(9,6), 2);
 }

 void Preprocedure::calibrate(Mat left, Mat right) {
      leftCamera.calibrate(left);
      rightCamera.calibrate(right);
      imshow("left", leftCamera.getResultImg());
      imshow("right", rightCamera.getResultImg());
 }

// Mat Preprocedure::getDisparityMap() {
//
//
// }
