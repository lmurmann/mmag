#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "laplacepyr.h"
#include "imsequence.h"


static const int c_npyr_levels = 7;

int main(int argc, char** argv) {
    using namespace lum;
    cv::Mat im = cv::imread("/Users/lum/code/6.869/mmag/img/funny.jpg");
    im.convertTo(im, CV_32FC3, 1.0/255);

//    LaplacePyr pyr = LaplacePyrBuild(im, c_npyr_levels);
//
//    cv::Mat canvas = LaplacePyrLayout(pyr);
//    cv::imshow("all", canvas);
//    cv::waitKey();
//    cv::destroyWindow("all");
//
//    cv::Mat reconstructed = LaplacePyrCollapse(pyr);
//    cv::imshow("reconstructed", reconstructed);
//    cv::waitKey();
//    cv::destroyWindow("reconstructed");


    std::vector<cv::Mat> seq = vidread("/Users/lum/code/6.869/mmag/videos/babysleeping_source.wmv");
    printf("nframes = %d\n", seq.size());

} 
