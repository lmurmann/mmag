//
//  videorenderer.cpp
//  mmag
//
//  Created by Lukas Murmann on 11/28/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#include "videorenderer.h"
#include <stdio.h>
#include <unistd.h>

#include <chrono>
#include <cmath>
#include <OpenGL/gl3.h>
#include "imsequence.h"
#include "laplacepyr.h"
#include "lum_glutil.h"
#include "lum_glcv_util.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace lum {
    videorenderer::videorenderer(const std::string & basepath)
    :m_basepath{basepath} {
        std::string vshader_src;
        std::string fshader_src;
        if (get_shader_src(basepath + "/shaders/quadrender.vert", &vshader_src)) {
            fprintf(stderr, "Error loading vertex shader");
            return;
        }
        if (get_shader_src(basepath + "/shaders/quadrender.frag", &fshader_src)) {
            fprintf(stderr, "Error loading fragment shader");
            return;
        }

        m_program_id = compile_shaders(vshader_src, fshader_src);
        if (m_program_id == 0) {
            fprintf(stderr, "Error compiling shadrs");
            return;
        }

        // geometry
        glGenVertexArrays(1, &m_varray_id);
        glBindVertexArray(m_varray_id);

        // Vertex Array
        glGenBuffers(1, &m_attrbuff_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_attrbuff_id);

        GLfloat pts[] = {-1,-1,0, 0,1,
                         1,-1,0, 1, 1,
                         1,1,0, 1, 0,
                        -1,1,0, 0, 0};
        glBufferData(GL_ARRAY_BUFFER, (20)*sizeof(GLfloat), pts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0); // xy
        glEnableVertexAttribArray(1); // uv
        // describe buffer layout, store in vertex array object
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

        // Elements Array
        glGenBuffers(1, &m_idxbuff_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxbuff_id);
        GLuint data[] = {0,1,2,2,3,0};
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 5*sizeof(GLuint), data, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        int err = glGetError();
        if (err) {
            fprintf(stderr, "glGetError() after creating renderer: %d\n", err);
        }

        glGenTextures(1, &m_texture_id);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //m_canvas_pyr = LaplacePyr();

        m_capture_buff = cv::Mat(480, 640, CV_32FC1);
        m_nlevels = LaplacePyrNumLevelsForSize(640, 480);
        //m_nlevels = 4;
        m_xseq.resize(3);
        m_yseq.resize(3);
        for (int i = 0 ; i < 3 ; ++i) {
            m_xseq[i] = LaplacePyr(m_capture_buff, m_nlevels);
            m_yseq[i] = LaplacePyr(m_capture_buff, m_nlevels);
            std::fill(m_xseq[i].data.begin(), m_xseq[i].data.end(), 0);
            std::fill(m_yseq[i].data.begin(), m_yseq[i].data.end(), 0);
        }

        m_b = {0.3590, 0, -0.3590};
        m_a = {1.0000, -1.1539, 0.2820};

        // pulse @30fps
        m_b = {0.0205,         0,   -0.0205};
        m_a = {1.0000, -1.9247, 0.9590};
    }

    videorenderer::~videorenderer() {
        glDeleteProgram(m_program_id);
        glDeleteBuffers(1, &m_attrbuff_id);
        glDeleteBuffers(1, &m_idxbuff_id);
        glDeleteVertexArrays(1, &m_varray_id);
        glDeleteTextures(1, &m_texture_id);
    }

    void videorenderer::set_alpha(float alpha) {
        m_alpha = alpha;
    }

    void videorenderer::set_spatial_cutoff(int level) {
        m_cutoff_level = level;
    }

    void videorenderer::set_bandpass(float lo, float hi) {
        printf("Bandpass not implemented. FIR coefficients are hardcoded around 1Hz\n");
    }

    void videorenderer::draw(double now) {
        glClearColor(0.0f, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        if (m_playback_start_s == 0) {
            m_playback_start_s = now;
        }

        double elapsed_s = now - m_last_frame;
        m_last_frame = now;
        printf("Time since frame %.1fms\n", elapsed_s * 1000);

        // left-shift buffers

        for (int i = 0 ; i < 2 ; ++i) {
            m_xseq[i] = m_xseq[i+1];
            m_yseq[i] = m_yseq[i+1];
        }

        auto be4 = std::chrono::system_clock::now();
        m_capture.read(m_capture_buff);
        auto after = std::chrono::system_clock::now();

        double just_filter_ms = std::chrono::duration_cast<std::chrono::microseconds>(after - be4).count() / 1000.f;
        printf("Read took %.1f\n", just_filter_ms);
        cv::cvtColor(m_capture_buff, m_capture_buff, CV_BGR2Lab);

        cv::Mat lab[3];
        cv::split(m_capture_buff, lab);

        m_xseq.back() = LaplacePyr(lab[0], m_nlevels);
        pyrseq_filter(m_xseq, m_yseq, m_b, m_a);

        // reuse m_xseq[0]. Will be shifted out next draw call.
        LaplacePyr& fr = m_xseq.front();
        for (int l = 0 ; l < fr.nlevels() ; ++l) {
            float lambda_factor = (l + 1.0) / (m_cutoff_level + 1);
            lambda_factor = fmin(1.0, lambda_factor);
            fr.level(l) = m_xseq[2].level(l) + lambda_factor * m_alpha* m_yseq[2].level(l);
        }


        lab[0] = LaplacePyrCollapse(fr);
        cv::merge(lab, 3, m_capture_buff);
        cv::cvtColor(m_capture_buff, m_capture_buff, CV_Lab2BGR);
        lum::upload_mat(m_texture_id, m_capture_buff);

        glBindVertexArray(m_varray_id);
        glUseProgram(m_program_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

    void videorenderer::play(double time) {
        m_playback_start_s = time;
    }
}
