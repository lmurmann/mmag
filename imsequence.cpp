//
//  imsequence.cpp
//  vmag
//
//  Created by Lukas Murmann on 11/28/14.
//
//

#include "imsequence.h"
#include <opencv2/highgui/highgui.hpp>

namespace lum{
    size_t pyrsequence::nframes() const {
        return frame.size();
    }
    size_t imsequence::nframes() const {
        return frame.size();
    }
    pyrsequence::pyrsequence(const imsequence &imseq) {
        int n_levels = 0;
        if (imseq.nframes() > 0) {
            n_levels = LaplacePyrNumLevelsForSize(imseq.frame[0].cols, imseq.frame[0].rows);
        }
        for (const cv::Mat & frm : imseq.frame) {
            frame.push_back(LaplacePyrBuild(frm, n_levels));
        }
    }

    imsequence vidread(const std::string & path) {
        using namespace cv;
        imsequence ret;
        cv::VideoCapture cap(path); // open the default camera
        if(!cap.isOpened()) {
            return ret;
        }
        ret.fps = cap.get(CV_CAP_PROP_FPS);
        assert(ret.fps > 0);

        int i = 0;
        for(Mat frame; cap.read(frame);)
        {
            frame.convertTo(frame, CV_32FC3, 1.0/255);
            ret.frame.push_back(frame);
            if (i++  > 127) {
                break;
            }
        }
        return ret;
    }
}
