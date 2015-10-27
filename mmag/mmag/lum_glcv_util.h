//
//  lum_glcv_util.h
//  mmag
//
//  Created by Lukas Murmann on 11/28/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#ifndef __mmag__lum_glcv_util__
#define __mmag__lum_glcv_util__

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <OpenGL/gl3.h>

namespace lum{
    void upload_mat(GLuint texture_id, const cv::Mat & im);
}

#endif /* defined(__mmag__lum_glcv_util__) */
