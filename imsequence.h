//
//  imsequence.h
//  vmag
//
//  Created by Lukas Murmann on 11/28/14.
//
//

#ifndef __vmag__imsequence__
#define __vmag__imsequence__

#include "laplacepyr.h"
#include <vector>
#include <opencv2/core/core.hpp>

namespace lum{
    class imsequence {
    public:
        std::vector<cv::Mat> frame;
        size_t nframes() const;
        int fps;
    };
    class pyrsequence {
    public:
        pyrsequence() = default;
        explicit pyrsequence(const imsequence &);
        std::vector<LaplacePyr> frame;
        size_t nframes() const;
    };

    imsequence vidread(const std::string & path);

    /* Compute nuber of float elements in a pyr sequence */
    size_t pyrseq_buffsz(const pyrsequence &);
    void pyramid_frame_from_spektrum(LaplacePyr& pyr, int t, const std::vector<float>& spektrum);

    void pyrseq_filter(const std::vector<LaplacePyr> & inpyr, std::vector<LaplacePyr>& outpyr, const std::vector<float> b,
                        const std::vector<float> a);
    void filter(const std::vector<float> & x, std::vector<float> & y,
                const std::vector<float> & b, const std::vector<float> & a);

    void filter(const std::vector<cv::Mat> & x, std::vector<cv::Mat> & y,
                const std::vector<float> & b, const std::vector<float> & a);
}

#endif /* defined(__vmag__imsequence__) */
