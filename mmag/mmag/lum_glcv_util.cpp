//
//  lum_glcv_util.cpp
//  mmag
//
//  Created by Lukas Murmann on 11/28/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#include "lum_glcv_util.h"


namespace lum{
    void upload_mat(GLuint texture_id, const cv::Mat & im) {
        GLuint err = glGetError();
        if (err) {
            fprintf(stderr, "Before texture upload: %d\n", err);
        }
        assert(im.type() == CV_32FC3); // expect BGR floating point image for simplicity
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.cols, im.rows, 0, GL_BGR, GL_FLOAT, im.data);
        err = glGetError();
        if (err) {
            fprintf(stderr, "After texture upload: %d\n", err);
        }
    }
}