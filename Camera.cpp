#include "Camera.hpp"

void Camera::Initialize(Size size, int index){
    borderSize = size;
    setFileList(index);
    addChessboardPoints();
}
void Camera::setFileList(int fileIndex){
        String imgFolder = to_string(fileIndex) + "/" + to_string(fileIndex) + "-";
        for (int j = 1; j <= 20; j++) {
            String filename = "/Users/yangenci/Desktop/";
            filename = filename + imgFolder + to_string(j) + ".bmp";
            fileList.push_back(filename);
        }
}

void Camera::addChessboardPoints(){
    vector<Point2f> srcCandidateCorners;
    vector<Point3f> dstCandidateCorners;

    // Initialize the dist matrix
    for(int i=0; i<borderSize.height; i++){
        for(int j=0; j<borderSize.width; j++){
            dstCandidateCorners.push_back(Point3f(i, j, 0.0f));
        }
    }
    cout<<borderSize<<endl;

    for (int i = 0; i < fileList.size(); i++) {
        //Read all the images for collibration and set it to gray scale
        Mat image = imread(fileList[i],CV_LOAD_IMAGE_GRAYSCALE);


        // @findChessboardCorners
        //    Return boolean type.
        //    The opencv function:(input image, chess board size, OutputArray corners, flags(option)).
        findChessboardCorners(image, borderSize, srcCandidateCorners);

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

    }
}

void Camera::calibrate(Mat img){

    Size imgSize = img.size();
    Mat camMat, distCoeffs, map1, map2;
    vector<Mat> rvecs, tvecs;
    calibrateCamera(dstPoints, srcPoints, imgSize, camMat, distCoeffs, rvecs, tvecs);
    initUndistortRectifyMap(camMat, distCoeffs, Mat(), Mat(), imgSize, CV_32F, map1, map2);
    remap(img, resultImg, map1, map2, INTER_LINEAR);
    cameraMatrix = camMat;
    distCoeffsMatrix = distCoeffs;
//     rotationMatrix = rvecs;
//     translateMatrix = tvecs;
//
}
Mat Camera::getCameraMat() {
    return cameraMatrix;
}
Mat Camera::getRotationMat() {
    return rotationMatrix;
}
Mat Camera::getTraslateMat() {
    return translateMatrix;
}
Mat Camera::getDistory() {
    return distCoeffsMatrix;
}
Mat Camera::getResultImg() {
    return resultImg;
}
