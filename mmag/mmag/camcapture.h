//
//  camcapture.h
//  mmag
//
//  Created by Lukas Murmann on 12/6/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#ifndef __mmag__camcapture__
#define __mmag__camcapture__

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>

namespace lum {
    class camcapture {
    public:
        camcapture();
        void read(cv::Mat & frame);

    private:
        cv::VideoCapture m_input;
    };
}

#endif /* defined(__mmag__camcapture__) */
