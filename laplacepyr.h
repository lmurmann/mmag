//
//  laplacepyr.h
//  vmag
//
//  Created by Lukas Murmann on 11/28/14.
//
//

#ifndef __vmag__laplacepyr__
#define __vmag__laplacepyr__

#include <stdio.h>
#include <vector>
#include <opencv2/core/core.hpp>

namespace lum {
    class LaplacePyr {
    public:
        LaplacePyr();
        LaplacePyr(const cv::Mat& frame, int n_levels);
        // not really const since retval points to data
        cv::Mat level(int level) const;

        std::vector<float> data;

        int nlevels() const {return m_nlevels;};
        size_t base_w() const {return m_l0w;}
        size_t base_h() const {return m_l0h;}
        int type() const {return CV_32FC3;}
        size_t nelements() const { return data.size(); }
    private:
        size_t m_nchannels;
        std::vector<size_t> m_level_origin;
        size_t m_l0w = 0;
        size_t m_l0h = 0;
        int m_nlevels = 0;
    };

    // compute how many levels can be computed from a source image
    int LaplacePyrNumLevelsForSize(int w, int h);

    size_t LaplacePyrNumElements(int w, int h, int channels, int nlevels);

    LaplacePyr LaplacePyrBuild(const cv::Mat & im, int n_levels);
    cv::Mat    LaplacePyrLayout(const LaplacePyr & pyr);
    cv::Mat    LaplacePyrCollapse(const LaplacePyr & pyr);

}

#endif /* defined(__vmag__laplacepyr__) */
