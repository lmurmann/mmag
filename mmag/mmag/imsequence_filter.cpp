//
//  imsequence_fft.cpp
//  mmag
//
//  Created by Lukas Murmann on 11/29/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#include <stdio.h>
#include "imsequence.h"
#include <opencv2/core/core.hpp>


namespace lum {
    size_t pyrseq_buffsz(const pyrsequence &pyrseq) {
        size_t n_frames = pyrseq.frame.size();
        if (n_frames == 0) {
            return 0;
        }
        const LaplacePyr& p0 = pyrseq.frame.front();
        return p0.nelements() * n_frames;
    }
    
    void pyramid_frame_from_spektrum(LaplacePyr& pyr, int t, const std::vector<float>& spektrum) {
        size_t n_frames = spektrum.size() / pyr.data.size();
        int j = 0;
        for (int i = t; i < spektrum.size() ; i += n_frames) {
            float f = spektrum[i];
            pyr.data[j++] = f;

        }
        assert(j == pyr.data.size());
    }

    /* The order of the filter is derived from the length of the input lists.
       the results of this operation are written to outpyr.back(). inpyr.front() is 
       the input frame that was computed 'n' frames back.
     */
    void pyrseq_filter(const std::vector<LaplacePyr> & inpyr, std::vector<LaplacePyr>& outpyr, const std::vector<float> b,
                        const std::vector<float> a) {
        // let's be a bit restrictive for now.
        const size_t n = inpyr.size();
        assert(n == 3);
        assert(a.size() == n);
        assert(b.size() == n);
        assert(a[0] == 1.0f);

        std::vector<cv::Mat> x(n);
        std::vector<cv::Mat> y(n);
        
        const LaplacePyr& latest_in = inpyr.back();
        LaplacePyr& latest_out = outpyr.back();
        const size_t nelements = latest_in.data.size();

        for (int l = 0 ; l < latest_in.nlevels(); ++l) {
            x[0] = inpyr[0].level(l);
            x[1] = inpyr[1].level(l);
            x[2] = inpyr[2].level(l);
            y[0] = outpyr[0].level(l);
            y[1] = outpyr[1].level(l);
            filter(x, y, b, a);

            y[2].copyTo(latest_out.level(l));
        }

    }
    void filter(const std::vector<float> & x, std::vector<float> & y,
                const std::vector<float> & b, const std::vector<float> & a) {
        const size_t n = 3;
        y[n-1] = b[0]*x[n-1] + b[1]*x[n-2] + b[2]*x[n-3] // x[0]
        -a[1]*y[n-2] - a[2]*y[n-3]; // y[0]
        assert(!isnan(y[n-1]));
        assert(!isinf(y[n-1]));
    }

    void filter(const std::vector<cv::Mat> & x, std::vector<cv::Mat> & y,
                const std::vector<float> & b, const std::vector<float> & a) {
        const size_t n = 3;
        y[n-1] = b[0]*x[n-1] + b[1]*x[n-2] + b[2]*x[n-3] // x[0]
        -a[1]*y[n-2] - a[2]*y[n-3]; // y[0]
    }
}