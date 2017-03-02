#include "Camera.hpp"

void Camera::Initialize(Size size, int index){
    borderSize = size;
    setFileList(index);
    addChessboardPoints();
}
void Camera::setFileList(int fileIndex){
    String imgFolder = to_string(fileIndex) + "/" + to_string(fileIndex) + "-";
    String index = "";
    fileIndex == 1 ? index = "left4/" : index = "right4/";
    for (int j = 1; j < 23; j++) {
        String filename = "/Users/yangenci/Desktop/Data/";
        filename = filename + index + to_string(j) + ".jpeg";
        fileList.push_back(filename);
        cout<<filename<<endl;
    }
}

void Camera::addChessboardPoints(){
    vector<Point2f> srcCandidateCorners;
    vector<Point2f> corners;

    vector<Point3f> dstCandidateCorners;

    // Initialize the dist matrix
    for(int i=0; i<borderSize.height; i++){
        for(int j=0; j<borderSize.width; j++){
            dstCandidateCorners.push_back(Point3f(i*28, j*28, 0.0f));
        }
    }


    for (int i = 0; i < fileList.size(); i++) {
        //Read all the images for collibration and set it to gray scale
        Mat image = imread(fileList[i],CV_LOAD_IMAGE_GRAYSCALE);

        Mat resized;
        resize(image, resized, Size(image.cols * 2, image.rows * 2), 0.0, 0.0, INTER_LINEAR);
        findChessboardCorners(resized, borderSize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

          for (int k = 0; k < corners.size(); ++k) {
              corners[k].x /= 2.0;
              corners[k].y /= 2.0;
              srcCandidateCorners.push_back(corners[k]);
          }
        // @findChessboardCorners
        //    Return boolean type.
        //    The opencv function:(input image, chess board size, OutputArray corners, flags(option)).
        // findChessboardCorners(image, borderSize, srcCandidateCorners);

        // @TermCriteria (Type, maxCount, epsilon)
        //    Type: The type of termination criteria
        //    maxCount: The maximum number of iterations or elements to compute.
        //    epsilon: The desired accuracy or change in parameters at which the iterative algorithm stops.
        TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);

        // @cornerSubPix (Input Image, Corners, Window Size, ZeroZone, Criteria)
        //    To search the corner of the image, and set the end condition by using TermCriteria result.
        cornerSubPix(image, srcCandidateCorners, Size(5,5), Size(-1,-1), param);

        // If the corner detection result count is match to the count that we input(by borderSize),
        // We then consider it as the collibration point
        if(srcCandidateCorners.size() == borderSize.area()){
            srcPoints.push_back(srcCandidateCorners);
            dstPoints.push_back(dstCandidateCorners);
        }
        else
            cout<<"not found :"<<fileList[i]<<endl;
        srcCandidateCorners.clear();
    }
}

void Camera::calibrate(Mat img){
    cout<<"in camera calibration"<<endl;
    imgSize = img.size();
    Mat camMat, distCoeffs, map1, map2;
    vector<Mat> rvecs, tvecs;
    float rms = calibrateCamera(dstPoints, srcPoints, imgSize, camMat, distCoeffs, rvecs, tvecs);
    cout<<"rms: "<<rms;
    initUndistortRectifyMap(camMat, distCoeffs, Mat(), Mat(), imgSize, CV_32F, map1, map2);
    remap(img, resultImg, map1, map2, INTER_LINEAR);
    cameraMatrix = camMat;
    distCoeffsMatrix = distCoeffs;
}
Mat Camera::getCameraMat() {
    return cameraMatrix;
}
Mat Camera::getRvec() {
    return rvec;
}
Mat Camera::getTvec() {
    return tvec;
}
Mat Camera::getDistory() {
    return distCoeffsMatrix;
}
Mat Camera::getResultImg() {
    return resultImg;
}
Size Camera::getImgSize() {
    return imgSize;
}
vector<vector<Point3f>> Camera::getObjectPoint() {
    return dstPoints;
}
vector<vector<Point2f>> Camera::getImgPoint() {
    return srcPoints;
}
