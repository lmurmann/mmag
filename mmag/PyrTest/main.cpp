//
//  main.cpp
//  PyrTest
//
//  Created by Lukas Murmann on 11/29/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "laplacepyr.h"

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        printf("Usage: %s <image_filename>\n", argv[0]);
        return -1;
    }
    cv::Mat im = cv::imread(argv[1]);
    im.convertTo(im, CV_32FC3, 1.0/255);
    cv::cvtColor(im, im, CV_BGR2Lab);
    cv::Mat luminance;
    cv::extractChannel(im, luminance, 0);
    cv::imshow("fun", luminance);
    cv::waitKey();
    int nlevels = lum::LaplacePyrNumLevelsForSize(im.cols, im.rows);
    lum::LaplacePyr pyr(luminance, nlevels);

    for (int l = 0 ; l < nlevels ; ++l) {
        cv::imshow("fun", pyr.level(l));
        auto m = cv::mean(pyr.level(l));
        printf("Level %d with m= %.2f\n", l,m[0]);
        cv::waitKey();
    }
    return 0;
}
