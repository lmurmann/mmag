//
//  laplacepyr.cpp
//  vmag
//
//  Created by Lukas Murmann on 11/28/14.
//
//

#include "laplacepyr.h"
#include <cassert>
#include <opencv2/imgproc/imgproc.hpp>

namespace lum {

    LaplacePyr::LaplacePyr() : LaplacePyr(cv::Mat(0,0, CV_32FC3), 1) {
    }

    LaplacePyr::LaplacePyr(const cv::Mat& im, int n_levels) {
        m_nchannels = im.channels();
        assert(n_levels > 0);
        m_l0h = im.rows;
        m_l0w = im.cols;
        m_nlevels = n_levels;
        assert(m_l0h % (int)pow(2, n_levels - 1) == 0);
        assert(m_l0w % (int)pow(2, n_levels - 1) == 0);

        data.resize(LaplacePyrNumElements(m_l0w, m_l0h, m_nchannels, n_levels));


        std::vector<cv::Mat> gausspyr;
        cv::buildPyramid(im, gausspyr, n_levels - 1);

        m_level_origin.resize(n_levels);
        m_level_origin[0] = 0;
        for (int l = 1 ; l < n_levels ; ++l) {
            m_level_origin[l] = m_level_origin[l-1] + (m_l0w>>(l-1))*(m_l0h>>(l-1)) * m_nchannels;
        }


        for (int l = 0; l < n_levels-1; ++l) {
            cv::Mat blurry;
            cv::resize(gausspyr[l+1], blurry, level(l).size(), 0, 0, CV_INTER_CUBIC);
            cv::Mat diff = gausspyr[l] - blurry;
            diff.copyTo(level(l));
        }
        if (n_levels > 1) {
            gausspyr[n_levels -1].copyTo(level(n_levels - 1));
        }
    }
    cv::Mat LaplacePyr::level(int l) const{
        return cv::Mat((m_l0h>>l),(m_l0w>>l), m_nchannels == 3 ? CV_32FC3 : CV_32FC1, const_cast<float*>(&data[m_level_origin[l]]));
    }

    int LaplacePyrNumLevelsForSize(int w, int h) {
        int ret = 1;
        while (w % 2 == 0 && h % 2 == 0) {
            ret += 1;
            w /=2;
            h/=2;
        }
        return ret;
    }
    size_t LaplacePyrNumElements(int w, int h, int nchan, int nlevels) {
        size_t total = 0;
        for (int i = 0 ; i < nlevels ; ++i) {
            total += w*h;
            w>>=1;
            h>>=1;
        }
        return total * nchan; // 3 channels;
    }

    LaplacePyr LaplacePyrBuild(const cv::Mat & im, int n_levels) {
        return LaplacePyr(im, n_levels);
    }
    cv::Mat LaplacePyrLayout(const LaplacePyr & pyr) {
        int l0w = pyr.base_w();
        int l0h = pyr.base_h();
        int l0t = pyr.type();
        cv::Mat canvas = cv::Mat(l0h, l0w * 1.5 + 1, l0t);
        int xo = l0w;
        int yo = 0;
        pyr.level(0).copyTo(canvas(cv::Rect(0, 0, l0w, l0h)));
        for (int i = 1 ; i < pyr.nlevels() ; ++i) {
            int level_w = pyr.level(i).cols;
            int level_h = pyr.level(i).rows;
            pyr.level(i).copyTo(canvas(cv::Rect(xo, yo, level_w, level_h)));
            yo += level_h;
        }
        return canvas;
    }
    cv::Mat LaplacePyrCollapse(const LaplacePyr & pyr) {
        int w = pyr.base_w();
        int h = pyr.base_h();
        int n = pyr.nlevels();
        cv::Mat coarse = pyr.level(pyr.nlevels() - 1);
        for (int i = pyr.nlevels() - 2; i >= 0 ; --i) {
            cv::Mat fine = pyr.level(i);
            cv::resize(coarse, coarse, fine.size(), 0, 0, CV_INTER_CUBIC);
            coarse += fine;
        }
        return coarse;
    }
}
