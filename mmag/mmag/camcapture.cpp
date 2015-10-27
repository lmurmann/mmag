//
//  camcapture.cpp
//  mmag
//
//  Created by Lukas Murmann on 12/6/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#include "camcapture.h"


namespace lum {
    camcapture::camcapture()
    :m_input(0){
        m_input.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        m_input.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
        m_input.set(CV_CAP_PROP_FPS, 30);
    }
    void camcapture::read(cv::Mat & frame) {
        m_input.read(frame);
        frame.convertTo(frame, CV_32FC3, 1.0/255);
    }
}