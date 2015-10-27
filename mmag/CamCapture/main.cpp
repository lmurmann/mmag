//
//  main.cpp
//  CamCapture
//
//  Created by Lukas Murmann on 12/6/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, const char * argv[]) {
    using namespace cv;
    // insert code here...
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);//1920/downscale);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);//1080/downscale);

    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        //resize(frame, frame, Size(frame.cols /4, frame.rows / 4));

        //assert(frame.type() == CV_32FC3);

        imshow("c", frame);
        waitKey(1);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
