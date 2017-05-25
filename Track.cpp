#include "headfile/Track.hpp"

void Track::update_imgs( Mat s, Mat c, int count ) {

   if ( count == 0 ) {
       capture.initialize( s, c );
       new_img.push_back( s );
       new_img.push_back( c );
       cvtColor( c, gray_image, CV_BGR2GRAY );
   }
   else {
       stereo = new_img[0].clone();
       color_image = new_img[1].clone();
       new_img.clear();
       new_img.push_back(s);
       new_img.push_back(c);
       capture.update_frame(c, s);
       if( count > 2 && !capture.target_exit() ) {
           search_target();
           target = capture.get_target();
        }
   }
}
void Track::tracking() {

    target.update_frame( stereo, color_image );
    target.refine_target();
    capture.cal_search_region();
    capture.set_Target( target );
    capture.do_tracking();

    Mat c = color_image.clone();
    Mat result;
    Mat color = color_image( capture.get_region_pos() );
    Mat ste_img = stereo( capture.get_region_pos() );
    cvtColor( ste_img, ste_img, CV_GRAY2BGR );

    double union_part = computRectJoinUnion( capture.get_region_pos(),capture.get_result() );

    if ( union_part > 0.3 )  putText( c, to_string(union_part), capture.get_region_pos().tl(), 1, 1, Scalar(0, 233, 120) );

    rectangle( c, capture.get_region_pos(), Scalar(0, 0, 255) );
    rectangle( c, capture.get_result(), Scalar(100, 255, 255) );
    track_result = capture.get_result();
    motion_img = capture.get_motion();
    data_count++;

    Mat g = gray_image.clone();
    Moments mo = moments( g(track_result) );
    Point center = Point( mo.m10/mo.m00 , mo.m01/mo.m00 );
    cvtColor( g, g, CV_GRAY2BGR );
    path.push_back(Point( center.x + track_result.tl().x, center.y + track_result.tl().y) );
}

float Track::computRectJoinUnion( Rect rc1, Rect rc2 ) {
    Point p1, p2;
    p1.x = std::max(rc1.x, rc2.x);
    p1.y = std::max(rc1.y, rc2.y);

    p2.x = std::min(rc1.x +rc1.width, rc2.x +rc2.width);
    p2.y = std::min(rc1.y +rc1.height, rc2.y +rc2.height);

    double AJoin = 0;
    if( p2.x > p1.x && p2.y > p1.y )
    {
        AJoin = (p2.x - p1.x)*(p2.y - p1.y);
    }
    float A1 = rc1.width * rc1.height;
    float A2 = rc2.width * rc2.height;
    double AUnion = (A1 + A2 - AJoin);

    if( AUnion > 0 )
        return (AJoin/AUnion);
    else
        return 0;
}
bool good_roi( Rect pso, Rect active ) {
    return false;
}
void Track::search_target() {
   capture.cal_roi();
}

bool Track::target_exit() {
  return capture.target_exit();
}

void Track::show_path() {
    Mat c = color_image.clone();
    rectangle( c, track_result, Scalar(0, 0, 255) );

    if ( path.size() > 15 )
        path.erase(path.begin());
    Mat router(gray_image.rows, gray_image.cols, CV_8UC1, Scalar(255));
    cvtColor(router, router, CV_GRAY2BGR);
    for (int i = 0; i < path.size(); i++) {
        circle(c, path[i], 3, Scalar(0, 0, 255-0.1*i), 5, CV_AA, 0);

        circle(router, path[i], 3, Scalar(255-20*i, 255-10*i, 255-10*i), 5, CV_AA, 0);
    }
    imshow("result", c);
    imshow("path", router);

}

Rect Track::result() {
  return track_result;
}

Mat Track::motion() {
  return motion_img;
}
