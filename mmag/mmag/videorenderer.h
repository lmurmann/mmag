//
//  videorenderer.h
//  mmag
//
//  Created by Lukas Murmann on 11/28/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#ifndef __mmag__videorenderer__
#define __mmag__videorenderer__

#include <string>
#include <opencv2/core/core.hpp>
#include "imsequence.h"
#include "camcapture.h"
#include <vector>

namespace lum {
    class videorenderer {
    public:
        explicit videorenderer(const std::string & basepath);
        ~videorenderer();
        /* absolute path */
        void draw(double time);

        void play(double time);

        void set_alpha(float alpha);
        void set_spatial_cutoff(int level);
        void set_bandpass(float lo, float hi);
    private:
        std::string m_basepath;

        float m_alpha = 0;
        cv::Mat m_capture_buff;
        int m_nlevels;
        int m_cutoff_level;
        std::vector<LaplacePyr> m_xseq;
        std::vector<LaplacePyr> m_yseq;
        std::vector<float> m_a;
        std::vector<float> m_b;
        camcapture m_capture;
        double m_playback_start_s = 0;
        double m_last_frame;
        unsigned int m_program_id;
        unsigned int m_texture_id;

        unsigned int m_varray_id;
        unsigned int m_attrbuff_id;
        unsigned int m_idxbuff_id;
    };
}

#endif /* defined(__mmag__videorenderer__) */
